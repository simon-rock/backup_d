#include "sysinfod.h"
#include <cstdlib>
#include <cstdio>
#include <sys/mount.h> // mount fun
#include "systools.h"
#include <fstream>
#include <sstream>
using std::ofstream;
using std::fstream;
using std::stringstream;
using std::ios;
sysinfod sysinfod::m_instance;

// 
static char* pbuf = 0;
#define BUFF_MAX 1024

sysinfod::sysinfod()
{
	// pbuf = (char*)malloc(sizeof(char)*BUFF_MAX);
	int size = BUFF_MAX;
	pbuf = (char*)malloc(sizeof(char)* BUFF_MAX);
}
sysinfod::~sysinfod()
{
	free(pbuf);
}
void sysinfod::init_disk_id()
{
	FILE   *stream;
	memset( pbuf, '\0', sizeof(char)*BUFF_MAX);
	stream = popen( "fdisk -l", "r" );
	string disk;
	string id;
	m_disk_id.clear();
	bool bfind_disk = false;

	string sbuf;
	while(fgets(pbuf, 1024,stream) != NULL)
	{
		sbuf = pbuf;
		if (sbuf.find("Disk /dev/sd") != string::npos ||
			sbuf.find("Disk /dev/hd") != string::npos)
		{
			string::size_type index = sbuf.find(":");
			disk = sbuf.substr(strlen("Disk "), index - strlen("Disk "));
			bfind_disk = true;
		}

		if (bfind_disk)
		{
			if (sbuf.find("Disk identifier:") != string::npos)
			{
				id = sbuf.substr(strlen("Disk identifier: "), strlen("0x00000000"));
				m_disk_id[disk] = id;
				bfind_disk = false;
			}
		}
	}

    // add disk info to pool
    /*
    for(map<string, string>::iterator item = m_disk_id.begin();
        item != m_disk_id.end();++item)
    {
        shared_ptr<disk_obj> o(new disk_obj);
        o->path = item->first;
        o->id = item->second;
        m_disk_pool.push_back(o);
    }
    */

//	fread( pbuf, sizeof(char), BUFF_MAX, stream);
	pclose( stream );
}
void sysinfod::init_Partition_path()
{
	FILE   *stream;
	memset( pbuf, '\0', sizeof(char)*BUFF_MAX);
	stream = popen( "mount -l", "r" );
	// if error return NULL
	string partition;
	string mountpath;
	m_path_Partition.clear();
	string::size_type index, space_index;

	string sbuf;
	while(fgets(pbuf, 1024,stream) != NULL)
	{
		sbuf = pbuf;
		if ((index = sbuf.find(" on ")) != string::npos)
		{
			partition = sbuf.substr(0, index);

			mountpath = sbuf.substr(index + strlen(" on "));

			if ((space_index = mountpath.find(" "))!= string::npos)
			{
				mountpath = mountpath.substr(0, space_index);
                // change mountpath -> partition => partition -> mountpath
                // m_path_Partition[mountpath] = partition;
                m_path_Partition[partition] = mountpath;
			}
			else
			{
				mountpath = "";
			}
		}
	}
    /*
    // add mount path info to disk pool
    for(vector<shared_ptr<disk_obj> >::iterator item = m_disk_pool.begin();
        item != m_disk_pool.end(); ++item)
    {
        (*item)->mount_path = m_path_Partition[(*item)->path + "1"];
    }
    */
//	fread( pbuf, sizeof(char), BUFF_MAX, stream);
	pclose( stream );
}

void sysinfod::init_sas_address()
{
   FILE *stream;
   //   pbuf = (char *)malloc(sizeof(char)* BUFF_MAX);
    memset(pbuf, '\0', sizeof(char)*BUFF_MAX);
    stream = popen("./lsiutil.x86_64 -s", "r");

    string sbuf;
    // ctrl
    //map<string, shared_ptr<ctrl_info> > ctrl_pool;
    // disk
    string sas_address;
    int num = 0;
    while(fgets(pbuf, 1024, stream) != NULL)
    {
        sbuf = pbuf;
        if (sbuf.find("EnclServ") != string::npos)
        {
            //            shared_ptr<sas_address_obj> o(new sas_address_obj);
            string::size_type index = sbuf.rfind(' ');
            if (index != string::npos)
            {
                //string tmp = sbuf.substr(index);
                //num = atoi(tmp.c_str());
                num = atoi(sbuf.substr(index).c_str());
                sbuf = sbuf.substr(0, index);
                if((index = sbuf.find_last_not_of(' ')) != string::npos)
                {
                    sbuf = sbuf.substr(0, index + 1);
                    if ((index = sbuf.rfind(' ')) != string::npos)
                    {
                        sas_address = sbuf.substr(index + 1);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }

            if (sas_address.size() > 12)
            {
                m_ctrl_sas_address[sas_address.substr(0, 12)] = sas_address;
                m_sas_address_num[sas_address] = num;
            }
            //m_ctrl_sas_address_pool.push_back(o);
            //cout << sas_address << ":" << num << endl;
            sas_address = "";
            num = 0;
        }
        else if (sbuf.find("Disk") != string::npos)
        {
            string::size_type index = sbuf.rfind(' ');
            if (index != string::npos)
            {
                num = atoi(sbuf.substr(index).c_str());
                sbuf = sbuf.substr(0, index);
                if((index = sbuf.find_last_not_of(' ')) != string::npos)
                {
                    sbuf = sbuf.substr(0, index + 1);
                    if ((index = sbuf.rfind(' ')) != string::npos)
                    {
                        sas_address = sbuf.substr(index + 1);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
                        
            m_sas_address_num[sas_address] = num;
            sas_address = "";
            num = 0;
        }
    }
    /*
    for(map<string, shared_ptr<ctrl_info> >::iterator item = ctrl_pool.begin();
    item != ctrl_pool.end(); ++item)
    {
        cout << item->first << " : "
             << item->second->_sas_address << " : "
             << item->second->_num << endl;   
    }
    */
    pclose(stream);
}
void sysinfod::print()
{
	map<string, string>::iterator item;
	cout << "-----disk_id-----" << endl;
	for(item = m_disk_id.begin(); item != m_disk_id.end(); ++item)
	{
		cout << item->first << " : " << item->second << endl;
	}
	cout << "-----Partition_path-----" << endl;
	for(item = m_path_Partition.begin(); item != m_path_Partition.end(); ++item)
	{
		cout << item->first << " : " << item->second << endl;
	}
    cout << "-----sas_address_num-----" << endl;
    for (map<string, int>::iterator item = m_sas_address_num.begin(); item != m_sas_address_num.end(); ++item)
    {
        cout << item->first << " : " << item->second << endl;
    }
    cout << "-----ctrl_sas_address-----"<< endl;
    for (item = m_ctrl_sas_address.begin(); item != m_ctrl_sas_address.end() ; ++item)
    {
        cout << item->first << " : " << item->second << endl;    
    }
    cout << "-----disk_pool-----" << endl;
    for (vector<shared_ptr<disk_obj> >::iterator item = m_disk_pool.begin();
         item != m_disk_pool.end(); ++item)
    {
        cout << "-------" << (*item)->path << endl;
        cout << (*item)->id << endl;
        cout << (*item)->mount_path << endl;
        cout << (*item)->sas_address << endl;
        cout << (*item)->physics_index << endl;
        cout << (*item)->ctrl_sas_address << endl;
        cout << (*item)->ctrl_physics_index << endl;
        cout << (*item)->ctrl_start_index << endl;
        cout << (*item)->asc_or_des << endl;
        cout << "pos : " << (*item)->get_pos() << endl;
    }

    cout << "-----jbod_info-----" << endl;
}

string sysinfod::get_diskid(string path, unsigned int pos_num)
{
	// 检测当前位置硬盘是否存在---通过联合disk_ctrl_info 信息
	// 是否格式化分区
	// 否=》格式化分区
	// 是否挂载
	// 是否挂载（目录已挂在则卸载再挂载）
	// 返回 disk_id, 发送消息显示当前使用的硬盘
    /*
	string tmp = m_path_Partition[path];
	if (!tmp.empty())
	{
		if (tmp[tmp.size() - 1] == '1')
		{
			tmp = tmp.substr(0, tmp.size() - 1);
			return m_disk_id[tmp]; 
		}
        else
            return "";
	}
	else
	{
		return "";
	}
*/
    // check disk with pos_num
    // umount path
    // mount disk
    // if sucess retunr disk_id
    for(vector<shared_ptr<disk_obj> >::iterator item = m_disk_pool.begin();
        item != m_disk_pool.end();
        ++item)
    {
        if ((*item)->get_pos() == pos_num)
        {
            umount_local_path(path);
            if (mount_local_ext4((*item)->path + "1", path))
            {
                return (*item)->id;   
            }
        }
    }
    return "";
}

int sysinfod::check_brick_src(string const &_brick_path, string const & _src_path)
{
	if(access(_src_path.c_str(),0)!=0)//access函数是查看文件是不是存在
	{
		// 迭代创建 yuyu
		if (MKDIR(_src_path.c_str()))//如果不存在就用mkdir函数来创建
		{
			return BK_INIT_SRC_CREATE_ER;
		}
	}

	string tmp = m_path_Partition[_src_path];
	if (tmp.empty())
	{
		if (connect_brick(_brick_path, _src_path) == BK_SUCESS)
		{
			return BK_SUCESS;
		}
		else
		{
			if (connect_brick(_brick_path, _src_path) == BK_SUCESS)
			{
				return BK_SUCESS;
			}
			else
			{
				return BK_INIT_SRC_MOUNT_ER;
			}
		}
		
	}
	else if(tmp == _brick_path)
	{
		return BK_SUCESS;
	}
	else
	{
		return BK_INIT_SRC_EXIST;
	}
}
int sysinfod::connect_brick(string const &_brick_path, string const & _src_path)
{
	// 确认 brick_ 是否 nfs 到本地 yuyu
	return BK_SUCESS;
}

string sysinfod::get_sas_address_by_disk(string const& p)
{
    FILE *stream;
    //    pbuf = (char*)malloc(sizeof(char)* BUFF_MAX);
    memset(pbuf, '\0', sizeof(char) *BUFF_MAX);
    string strcmd;
    strcmd =  "udevadm info -a -p $(udevadm info -q path -n";
    strcmd += p;
    strcmd += ") | grep ATTRS{sas_address}";
    stream = popen(strcmd.c_str(), "r");

    string sbuf;
    while(fgets(pbuf, 1024, stream) != NULL)
    {
        sbuf = pbuf;
        string::size_type index;
        if ((index = sbuf.find("\"0x")) != string::npos)
        {
            // cout << sbuf.substr(index + 3, sbuf.size() - index -3) << endl;
            pclose(stream);
            return sbuf.substr(index + 3, sbuf.size() - index - 5);
        }
        //cout << sbuf << endl;
    }
    pclose(stream);
    return "";
}

void sysinfod::init_disk_info()
{
    // clear system info
    m_disk_id.clear();
    m_path_Partition.clear();
    m_sas_address_num.clear();
    m_ctrl_sas_address.clear();
    m_disk_pool.clear();
    //
    
    init_disk_id();
    init_Partition_path();
    init_sas_address();
    // gen disk info
    string tmp;
    for(map<string, string>::iterator item = m_disk_id.begin();
        item != m_disk_id.end();
        ++item)
    {
        shared_ptr<disk_obj> o (new disk_obj);
        o->path = item->first;
        o->id   = item->second;
        o->mount_path = m_path_Partition[item->first + "1"];
        tmp = get_sas_address_by_disk(o->path);
        if (tmp != "")
        {
            o->sas_address = tmp;
            o->physics_index = m_sas_address_num[tmp];
            o->ctrl_sas_address = m_ctrl_sas_address[tmp.substr(0, 12)];
            o->ctrl_physics_index = m_sas_address_num[o->ctrl_sas_address];
            for(vector<jbod_info>::iterator item = m_vjbods.begin();
                item != m_vjbods.end();
                ++item)
            {
                if (o->ctrl_sas_address == (*item)._sas_address)
                {
                    o->ctrl_start_index = (*item)._start_num;
                    o->asc_or_des = (*item).asc_or_des;
                    break;
                }
            }
        }
        if (o->physics_index != -1 &&
            o->ctrl_physics_index != -1)
        {
            m_disk_pool.push_back(o);
        }
    }
}

bool sysinfod::mount_local_ext4(string const& _dev, string const & _mount_path)
{
    if (mount(_dev.c_str(),
              _mount_path.c_str(),
              "ext4",
              MS_MGC_VAL|MS_RDONLY, "") == 0)
    {
        ofstream o("/etc/mtab", ios::app);
        if (o)
        {
            o << _dev << " " << _mount_path << " ext4 rw 0 0" << endl;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool sysinfod::umount_local_path(string const& _path)
{
    fstream file("/etc/mtab", ios::in| ios::out);
    if (!file)
    {
        return false;
    }
    
    if (umount(_path.c_str())== 0)
    {
     
        stringstream buf;
        buf << file.rdbuf();

        file.close();
        ofstream out("/etc/mtab", ios::out);
        if (!out)
        {
            cout << "write file error" << endl;
            return true;
        }

        string line;
        while(getline(buf, line))
        {
            if (line.find(_path + " ") == string::npos)
            {
                out << line << endl;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

void sysinfod::set_jbod_info(vector<jbod_info>& _v)
{
    // init from mysql into m_vjbods
    m_vjbods = _v;
}
