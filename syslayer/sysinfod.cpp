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

// udev
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <libudev.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#undef asmlinkage
#ifdef __i386__
#define asmlinkage __attribute__((regparm(0)))
#else
#define asmlinkage 
#endif
#define UDEV_MAX(a,b) ((a) > (b) ? (a) : (b))
// udev process
int udevadm_monitor(struct udev *udev);
static volatile int event_count = 0;
static void print_device(struct udev_device *device, const char *source, int env)
{
    printf ("recive kernel event: %d\n", ++event_count);
}
static int udev_exit = 0;

sysinfod sysinfod::m_instance;

// 
static char* pbuf = 0;
#define BUFF_MAX 1024

sysinfod::sysinfod()
    :bfirst(true)
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

// path without last "/"
string sysinfod::get_diskid(string path, unsigned int pos_num)
{
	// ¼ì²âµ±Ç°Î»ÖÃÓ²ÅÌÊÇ·ñ´æÔÚ---Í¨¹ýÁªºÏdisk_ctrl_info ÐÅÏ¢
	// ÊÇ·ñ¸ñÊ½»¯·ÖÇø
	// ·ñ=¡·¸ñÊ½»¯·ÖÇø
	// ÊÇ·ñ¹ÒÔØ
	// ÊÇ·ñ¹ÒÔØ£¨Ä¿Â¼ÒÑ¹ÒÔÚÔòÐ¶ÔØÔÙ¹ÒÔØ£©
	// ·µ»Ø disk_id, ·¢ËÍÏûÏ¢ÏÔÊ¾µ±Ç°Ê¹ÓÃµÄÓ²ÅÌ
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
            else
            {
                cout << "mount disk [" <<(*item)->path.c_str() << "][" << path << "]" << endl;
                // mount disk failed
            }
        }
    }
    return "";
}

int sysinfod::check_brick_src(string const &_brick_path, string const & _src_path)
{
	if(access(_src_path.c_str(),0)!=0)//accessº¯ÊýÊÇ²é¿´ÎÄ¼þÊÇ²»ÊÇ´æÔÚ
	{
		// µü´ú´´½¨ yuyu
		if (MKDIR(_src_path.c_str()))//Èç¹û²»´æÔÚ¾ÍÓÃmkdirº¯ÊýÀ´´´½¨
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
	// È·ÈÏ brick_ ÊÇ·ñ nfs µ½±¾µØ yuyu
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

    // init monitor thread
    if(bfirst)
    {
        if (pthread_create(&m_thread_monitor, 0, montior_dev, (void*)this) == -1)
        {
            // start monitor failed
            cout << "start monitor failed" << endl;
        }
        
        if (pthread_create(&m_thread_flush, 0, flush_info, (void*)this) == -1)
        {
            // start flush thread failed
            cout << "start flush thread failed" << endl;
        }
        bfirst = false;
    }
    
    
}

bool sysinfod::mount_local_ext4(string const& _dev, string const & _mount_path)
{
    cout << "mount " << _dev << " " << _mount_path << endl;
    if (mount(_dev.c_str(),
              _mount_path.c_str(),
              "ext4",
              MS_MGC_VAL, "") == 0)
    {
        ofstream o("/etc/mtab", ios::app);
        if (o)
        {
            o << _dev << " " << _mount_path.c_str() << " ext4 rw 0 0" << endl;
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

void * sysinfod::montior_dev(void* _pthis)
{
    // if recive signle from kernel, init disk info
    struct udev *udev;
	udev = udev_new();
	if (udev == NULL)
    {
        udev_unref(udev);
        printf("start system monitor failed\n");
        return (void*)-1;
    }
	udevadm_monitor(udev);
	udev_unref(udev);
    printf("exit system monitor\n");
    return (void*)0;
}

void * sysinfod::flush_info(void* _pThis)
{
    // flush disk info
    int old_event_count = 0;
    while (!udev_exit)
    {
        if (old_event_count != 0 &&
            old_event_count == event_count)
        {
            old_event_count = event_count = 0;
            sysinfod::getInstance()->init_disk_info();
            sysinfod::getInstance()->print();
        }
        else
        {
            old_event_count = event_count;
            sleep(4);
        }
    }
}
int udevadm_monitor(struct udev *udev)
{
	// struct sigaction act;
	int env = 0;
	int print_kernel = 0;
	struct udev_monitor *kernel_monitor = NULL;
    struct timeval timeout={3,0};
	fd_set readfds;
	int rc = 0;
	if (!print_kernel) {
		print_kernel = 1;
	}
    /*
	if (getuid() != 0 && print_kernel) {
		fprintf(stderr, "root privileges needed to subscribe to kernel events\n");
		goto out;
	}
    */
	if (print_kernel) {
		kernel_monitor = udev_monitor_new_from_netlink(udev, "udev");
		if (kernel_monitor == NULL) {
			rc = 3;
			printf("udev_monitor_new_from_netlink() error\n");
			goto out;
		}
		if (udev_monitor_enable_receiving(kernel_monitor) < 0) {
			rc = 4;
			goto out;
		}
		printf("start the kernel uevent monitor\n");
	}
	
	while (!udev_exit) {
		int fdcount;
		FD_ZERO(&readfds);
		if (kernel_monitor != NULL)
			FD_SET(udev_monitor_get_fd(kernel_monitor), &readfds);
		fdcount = select(udev_monitor_get_fd(kernel_monitor)+1,
				 &readfds, NULL, NULL, &timeout);
		if (fdcount < 0) {
			if (errno != EINTR)
				fprintf(stderr, "error receiving uevent message: %m\n");
			continue;
		}
		if ((kernel_monitor != NULL) && FD_ISSET(udev_monitor_get_fd(kernel_monitor), &readfds)) {
			struct udev_device *device;
			device = udev_monitor_receive_device(kernel_monitor);
			if (device == NULL)
				continue;
			print_device(device, "UEVENT", env);
			udev_device_unref(device);
		}
 	}
out:
	udev_monitor_unref(kernel_monitor);
	return rc;
}

void sysinfod::stop_monitor()       // stop sys monitor
{
    udev_exit = 1;
    pthread_join(m_thread_monitor, NULL);
}
// user id check
// async update disk info
// close
