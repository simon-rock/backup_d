#include "disk_container.h"
#include "sysinfod.h"
#include "backup_task.h"
#include "msg_queue.h"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::ofstream;
using std::ios_base;
using std::ios;
using std::stringstream;
//#include <stat.h>
void disk_container::setdir(string dir, string _brick_id, vector<unsigned int>& _pos)
{
	if (dir.length() > 1 &&
		(dir[dir.length() - 1] == '\\' ||
		dir[dir.length() - 1] == '/'))
	{
		dir[dir.length() - 1] = '\0';
		//dir += DIS;
	}
	m_dir = dir;
	m_identity = _brick_id;
	m_pos = _pos;
}

int disk_container::init()
{
	// 调用格式化， 处理硬盘
	// 目录是否可达
	if (access(m_dir.c_str(), R_OK | W_OK) != 0)
	{
		LOG(WARNING) << "[acess m_dir.c_str() error]" << m_dir.c_str();
		FlushLogFiles(INFO);
		return BK_INIT_ERR;
	}
	// no test yuyu
	// 通过路径和和brick_id 检测并获得disk_id
	int ret = 0;
	for (vector<unsigned int>::iterator item = m_pos.begin(); item != m_pos.end(); ++item)
	{
		m_curr_disk_id = sysinfod::getInstance()->get_diskid(m_dir, *item);
		if (m_curr_disk_id.empty())
		{
			ret = BK_INIT_NO_DISK_MOUNT;
			continue;
			// return BK_INIT_NO_DISK_MOUNT;
		}
		// insert disk_id for
		if (m_db.add_disk_for_brick(m_identity, m_curr_disk_id) != BK_DB_SUCESS)
		{
			LOG(WARNING) << "[brick_id]" << m_identity << "[disk_id]" << m_curr_disk_id << "can't add disk to brick, this disk may used for surdoc backup, please check";
			ret = BK_INIT_NO_DISK_MOUNT;
			continue;
			// return BK_INIT_ADD_DISK;
		}
		else
		{
			ret = BK_SUCESS;
            m_curr_disk_id = *item;
            // show msg for new disk
            stringstream  show_msg;
            show_msg <<  "current use disk pos: " << m_curr_disk_pos;
            DEFINEMSGDATA(pmsg_data, BACKUP_BRICK_MSG);
            COPYDATA(pmsg_data->BRICK_ID, m_curr_disk_id.c_str(), m_curr_disk_id.size());
            COPYDATA(pmsg_data->MSG_SHOW, show_msg.str().c_str(), show_msg.str().size());
            POSTMESSAGE(BACKUP_BRICK_MSG,pmsg_data);
			break;
		}
	}
	if (ret != BK_SUCESS)
	{
		return ret;
	}
	//
	// 确定磁盘空间
	unsigned long long target_free = 0;
	unsigned long long target_total_free = 0;
	if (getdiskspaceinfo(m_dir.c_str(), target_free, m_target_space, target_total_free) != 0)
	{
		return BK_INIT_ERR;
	}
	if (target_free < (m_target_space*(1 - DISK_USED_LIMIT)))
	{
		return BK_INIT_NEED_DEL;  // 可能删除文件
	}
	m_target_space_used = m_target_space - target_free;
	m_target_space_limit = static_cast<unsigned long long>(m_target_space*DISK_USED_LINE);

	m_init_flag = true;
	return BK_SUCESS;
}

int disk_container::copy(const char *_srcfile, const char * _file)
{
	if (_srcfile == NULL || _file == NULL)
	{
		return BK_BACKUP_FILE_ERR;
	}
	
	int try_ret = SPACE_LIMIT;
	while (try_ret != BK_SUCESS)
	{
		try_ret = copy_try(_srcfile);
		switch (try_ret)
		{
		case BK_SUCESS:
			break;
		case BK_BACKUP_FILE_ERR:
			return BK_BACKUP_FILE_ERR;
		case BK_BACKUP_SPACE_WARNING:
			m_work_level = SPACE_WARNING;
			break;
		case BK_BACKUP_SPACE_LIMIT:
			m_work_level = SPACE_LIMIT;
			break;
		}

		if (try_ret != BK_SUCESS)
		{
            LOG(WARNING) << "*****task: " << m_curr_disk_id
                         << " disk pos: " << m_curr_disk_pos
                         << " is full*****";
            if (init() == BK_SUCESS)
            {
                LOG(INFO) << "task: " << m_curr_disk_id
                          << " new disk pos: " << m_curr_disk_pos;
            }
            else
            {
			/*
			当前没有硬件检测
			考虑此种情况 发送消息，停止备份
			*/
			task_control::Instance()->set_sem(m_identity);
			  // yuyu
			//MSG_DATA(BACKUP_BRICK_ERROR) *perr_msg_data= new MSG_DATA(BACKUP_BRICK_ERROR);
			//memset(perr_msg_data,0,sizeof(MSG_DATA(BACKUP_BRICK_ERROR)));
			//strncpy(perr_msg_data->BRICK_ID, m_identity.c_str(), m_identity.size() < sizeof(perr_msg_data->BRICK_ID) ?
			//	m_identity.size() : sizeof(perr_msg_data->BRICK_ID) - 1);
			//strcpy(perr_msg_data->MSG_SHOW, "DISK FULL");
			//POSTMESSAGE(MSG(BACKUP_BRICK_ERROR),perr_msg_data,sizeof(MSG_DATA(BACKUP_BRICK_ERROR)));
			DEFINEMSGDATA(perr_msg_data, BACKUP_BRICK_ERROR);
			COPYDATA(perr_msg_data->BRICK_ID, m_identity.c_str(), m_identity.size());
			COPYDATA(perr_msg_data->MSG_SHOW, "DISK FULL", sizeof("DISK FULL"));
			POSTMESSAGE(BACKUP_BRICK_ERROR, perr_msg_data);
			LOG(WARNING) << "[BRICK_ID]" << m_identity << "[DISK_ID]" << m_curr_disk_id << " : disk full";
			return BK_BACKUP_FILE_ERR;
            }
			/*
			do 
			{
				check();
			} while (init() != BK_SUCESS);
			m_work_level = SPACE_NORMAL;
			*/
		}
	}

	// file exist
	// this file have be Backed up to other disk;
	if (m_db.isbackuped("", _file) == BK_DB_EXIST)
	{
		return BK_SUCESS_FILE;
	}

	if (access((m_dir+_file).c_str(),0)==0 &&
		getfilesize(_srcfile) == getfilesize((m_dir+_file).c_str()))
	{
		if (m_db.add_file_log(m_curr_disk_id, _file) != BK_DB_SUCESS)
        {
            LOG(WARNING) << "[" << m_curr_disk_id << ":" << _file <<"]add file log failed";
        }
		return BK_SUCESS_FILE;
	}

	//if (strlen(_file)> 1)
	//{
		//m_db.isbackuped("", _file+1);

	
	//}
	
	// file is not exist
	ifstream   is; 
	ofstream   os;
	is.exceptions(ios_base::badbit);
	os.exceptions(ios_base::badbit);

	is.open( _srcfile,ios::in | ios::binary); 
	os.open( (m_dir+_file).c_str(), ios::out|ios::binary); 
	if (!is||!os)
    {
        LOG(WARNING) << "create folder [" << _srcfile << " ] failed";
   		return BK_BACKUP_FILE_ERR;
    }

	try   { 
		os   <<   is.rdbuf();
	} 
	catch   (ifstream::failure   e)   { 
		os.flush();
		os.close();
		is.close();
		// 删除文件
		remove((m_dir+_file).c_str());
        LOG(WARNING) << "create folder [" << _srcfile << " ] failed";
		return BK_BACKUP_FILE_ERR;
	} 

	os.flush();
	os.close();
	is.close();

	// 修改文件权限
	CHMOD(_srcfile, (m_dir+_file).c_str());

	// 更改可用空间
	m_target_space_used += getfilesize((m_dir+_file).c_str());

	// insert file info to db
	m_db.add_file_log(m_curr_disk_id, _file);
	//
	return BK_SUCESS_FILE;
}

int disk_container::copy_try(const char *_srcfile)
{
	long long size = 0;
	if (( size = getfilesize(_srcfile)) != -1)
	{
		if (size > (m_target_space - m_target_space_used))
		{
			return BK_BACKUP_SPACE_LIMIT;
		}
		else if (m_target_space_used > m_target_space_limit)
		{
			return BK_BACKUP_SPACE_WARNING;
		}
		else
			return BK_SUCESS;
	}
	return BK_BACKUP_FILE_ERR;
}

int disk_container::check()
{
	//****************************** 
	// 以逻辑硬盘为标示， 查询硬盘是否准备好
	// 否则阻塞
	//******************************
	return 0;
}

int disk_container::create_dir(const char *_folder)
{
	if (_folder == NULL)
	{
		return BK_BACKUP_FOLDER_ERR;
	}
	string a = (m_dir+_folder).c_str();
	if(access(a.c_str(),0)!=0)//access函数是查看文件是不是存在
	{
		if (MKDIR(a.c_str()))//如果不存在就用mkdir函数来创建
		{
            LOG(WARNING) << "create folder [" << _folder << " ] failed";
			return BK_BACKUP_FOLDER_ERR;
		}
	}
	return BK_SUCESS_FOLDER;
}
