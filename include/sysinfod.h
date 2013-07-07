#ifndef _SYSINFOD_H_
#define _SYSINFOD_H_
#include "com_inc.h"
#include <map>
#include <iostream>
#include <vector>
#ifdef SYS_WINDOWS
#include <memory>
#endif // SYS_WINDOWS

#ifdef SYS_LINUX
#include <tr1/memory>
#endif // SYS_LINUX

using std::tr1::shared_ptr;
using std::vector;
using std::cout;
using std::endl;
using std::map;

class sysindo;

typedef struct _ctrl_info
{
    string _sas_address;
    int _num;
_ctrl_info():_num(-1){}
}ctrl_info;
class disk_obj
{
 public:
 disk_obj():
            id(""),
            path(""),
            mount_path(""),
            sas_address(""),
            physics_index(-1),
            ctrl_sas_address(""),
            ctrl_physics_index(-1),
            ctrl_start_index(0),
            asc_or_des(true)
            {}
            ~disk_obj(){}
  int get_pos()
  {
      if (physics_index == -1 ||
          ctrl_physics_index == -1)
      {
          return -1;
      }
      else
          return asc_or_des?
              physics_index - ctrl_start_index + 1:
              ctrl_start_index - physics_index + 1;
  }
 private:
  string id;              // disk id
    string path;          // sys path
    string mount_path;    // first partition
    string sas_address;
    int physics_index;
    string ctrl_sas_address;
    int ctrl_physics_index;
    int ctrl_start_index; // start num of the jbod,  default 0
    bool asc_or_des;      // disk order of the jbod, default asc

    friend class sysinfod;
};

typedef struct _sas_address_obj
{
    //    bool m_ctrl_flag;
    string _sas_address;
    int _num;
_sas_address_obj():_num(-1)/*, m_ctrl_flag(false)*/{}
}sas_address_obj;
class sysinfod
{

public:
	static sysinfod* getInstance()
	{	
		return &m_instance;			
	}

    // jbod info
    class jbod_info
    {
    public:
        jbod_info():
            _start_num(0),
             asc_or_des(true){}
        ~jbod_info(){}
        string _sas_address;
        unsigned int _start_num;
        bool asc_or_des;        // ascending order or desending order
    };
    
     // jbod info   
    void set_jbod_info(vector<jbod_info> &_v);      // init jbod info from mysql(include sas_address, start num, ascordes)
    void init_disk_info();     // init all disk info
    void stop_monitor();       // stop sys monitor
#ifdef SYS_WINDOWS
    void print();// for test
#endif // SYS_WINDOWS
	string get_diskid(string path, unsigned int pos_num); // get disk_id by backuppath
	int check_brick_src(string const &_brick_path, string const & _src_path);
	int connect_brick(string const &_brick_path, string const & _src_path);
private:
    static void * montior_dev(void* _pThis);
    static void * flush_info(void* _pThis);
    
    string get_sas_address_by_disk(string const& p);
	static sysinfod m_instance;
	sysinfod();
	~sysinfod();
	sysinfod(const sysinfod&);
	sysinfod& operator = (const sysinfod&);
	// fun
	void init_disk_id();        // init disk => id
	void init_Partition_path(); // init partition => path
    void init_sas_address();    // init all disk and ctrl sas address
    bool mount_local_ext4(string const& _dev, string const & _mount_path);
    bool umount_local_path(string const& _path);
	map<string, string> m_disk_id, m_path_Partition;
	//map<string, string> m_disk_path;
    vector<shared_ptr<disk_obj> >m_disk_pool;
    //vector<shared_ptr<sas_address_obj> > m_sas_address_pool;
    map<string, int> m_sas_address_num;
    //vector<shared_ptr<sas_address_obj> > m_ctrl_sas_address_pool;
    map<string, string> m_ctrl_sas_address;
    vector<jbod_info> m_vjbods;

    // monitor sys dev
    pthread_t m_thread_monitor;
    pthread_t m_thread_flush;
    bool bfirst;    // first init diks info 
};
#endif //_SYSINFOD_H_
