#include "backup_worker.h"
#include "systools.h"
#include "backup_task.h"
#include "msg_queue.h"
#include "sysinfod.h"
#ifdef SYS_LINUX
// not use in linux
struct _finddata_t
{
	long long time_write;
	int attrib;
};
#define _A_SUBDIR 0
#endif // SYS_LINUX

int backup_worker::init()
{
	if (access(m_src_path.c_str(), 04) != 0)
	{
		LOG(WARNING) << "[acess m_src_path.c_str() error]" << m_src_path.c_str();
		FlushLogFiles(INFO);
		return BK_INIT_ERR;
	}

	m_frame.set_opt( file_frame::process, _proc);
	m_frame.set_opt( file_frame::filter, _filter);
	m_frame.set_opt( file_frame::statistic, _statistic);
	m_frame.set_opt( file_frame::showstatistic, _show);
	m_frame.set_opt( file_frame::data, this);
	m_frame.add_dir(m_src_path.c_str());
	// check brick src yuyu
	//	int ret = init_src();   // yuyu
	int ret = BK_SUCESS;
	if (ret != BK_SUCESS)
	{
		return ret;
	}
	return init_target();
}

int backup_worker::start()
{	
	if (!m_target.prepare() || 
		!m_frame.init())
	{
		return BK_INIT_NEEDED;
	}

	bool bfirstrun = true;
	while (1)
	{
		// 修正备份 end time
		time_t curr;
		time(&curr);
		// BACKUPSPACE h every time
		//m_end_time = curr - m_space_time;
		int warningcount = 0;
		
		if (bfirstrun)
		{
			bfirstrun = false;
			m_end_time = curr - FIRSTBACKUPSPACEBEFCURR;
			if (m_lastbackup_time > m_end_time)
			{
				m_end_time = (m_lastbackup_time + BACKUPSPACE)< curr - m_space_time? m_lastbackup_time + BACKUPSPACE : curr - m_space_time;
			}
		}
		else
		{
			m_end_time = (m_lastbackup_time + BACKUPSPACE)< curr - m_space_time? m_lastbackup_time + BACKUPSPACE : curr - m_space_time;
		}
		if ((m_end_time - m_lastbackup_time) < MINBACKUPSPACE)
		{
			if(!(warningcount++%60))
			{
				LOG(INFO) << "[BRICK_ID]" << m_brick_id << "[BEGIN]" << m_lastbackup_time  << "[END]" <<  m_end_time << "[msg] less BACKUPSPACE";
				FlushLogFiles(INFO);
			}
			
			usleep(1000000*30);
			if (stop()) return BK_STOP;
			continue;
		}
		
		DEFINEMSGDATA(pmsg_data, BACKUP_BRICK_MSG);
		COPYDATA(pmsg_data->BRICK_ID, m_brick_id.c_str(), m_brick_id.size());
		COPYDATA(pmsg_data->MSG_SHOW, ctime(&m_lastbackup_time), strlen(ctime(&m_lastbackup_time)));
		POSTMESSAGE(BACKUP_BRICK_MSG,pmsg_data);

		LOG(INFO) << "[BRICK_ID]" << m_brick_id << "[BEGIN]" << m_lastbackup_time  << "[END]" <<  m_end_time ;
		FlushLogFiles(INFO);
		if (m_frame.start() == FRAME_STOP)
		{
			return BK_STOP;
			//break;
		}
		
		m_frame.finish();
		m_db.update_backup_time(m_brick_id, m_lastbackup_time);
		m_lastbackup_time = m_end_time;
	}
}

bool backup_worker::stop()
{
	return task_control::Instance()->wait_sem(m_brick_id);
}

//int backup_worker::restore()
//{
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
int backup_worker::init_target()
{
	return m_target.init();
}

int backup_worker::init_src()
{
	return sysinfod::getInstance()->check_brick_src(m_brick_path, m_src_path);
}
//int backup_worker::pause()
//{
//	return 0;
//}
//////////////////////////////////////////////////////////////////////////
int backup_worker::_proc ( string SourcePath, string DestinationPath, string path, bool folder, void* pdata )
{
	backup_worker* p = static_cast<backup_worker*>(pdata);
	if (folder)
	{
		return p->m_target.create_dir(path.c_str());
	}
	else
	{
//		LOG(INFO) << "[file_path]" << (SourcePath + path).c_str() << "[to]" << path.c_str();
		return p->m_target.copy((SourcePath + path).c_str(), path.c_str());
	}
}
int backup_worker::_statistic ( int ret, void* pdata)
{
	backup_worker* p = static_cast<backup_worker*>(pdata);
	switch(ret)
	{
	case BK_BACKUP_FOLDER_ERR:
		p->count_folder_err ++;
		break;
	case BK_BACKUP_FILE_ERR:
		p->count_file_err ++;
		break;
	case BK_SUCESS_FILE:
		p->count_success ++;
		break;
	case BK_SUCESS_FOLDER:
		break;
	default:
		break;
	}
	return 0;
}
int backup_worker::_show (void* pdata)
{
	backup_worker* p = static_cast<backup_worker*>(pdata);
//	cout << "[count_success]" << p->count_success << endl;
//	cout << "[count_file_err ]" << p->count_file_err << endl;
//	cout << "[count_folder_err]" << p->count_folder_err<< endl;
	return 0;
}
int backup_worker::_filter (struct _finddata_t*  pfileinfo, const char * file_path, void* pdata)
{
	backup_worker* p = static_cast<backup_worker*>(pdata);
	if (p->stop())
	{
		return FRAME_STOP;
	}

	if (pfileinfo != NULL)
	{
		if ((pfileinfo->attrib & _A_SUBDIR) == _A_SUBDIR)
		{
			return FRAME_FILTER_IN;
		}
		if (pfileinfo->time_write >= p->m_lastbackup_time 
			&& pfileinfo->time_write < p->m_end_time )
		{
			return FRAME_FILTER_IN;
		}
	}
	else if (file_path != NULL)
	{
		if (isfolder(file_path))
		{
			return FRAME_FILTER_IN;
		}
		long long mtime = getfile_mtime(file_path);
//		LOG(INFO) << "[file_path]" << file_path << "[mtime]" << mtime << "[time_apce]" << p->m_lastbackup_time << " - " << p->m_end_time;
		if (mtime >= p->m_lastbackup_time 
			&& mtime < p->m_end_time )
		{
//			LOG(INFO) << "[file_path]" << file_path << "[FRAME_FILTER_IN]";
			return FRAME_FILTER_IN;
		}
	}
	return FRAME_FILTER_E;
	
}
