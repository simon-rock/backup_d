#ifndef _DISK_CONTAINER_H_
#define _DISK_CONTAINER_H_
#include "com_inc.h"
#include "systools.h"
#include "db_exec.h"
#include <vector>
using std::vector;
enum _lev
{
	SPACE_NORMAL = 0,
	SPACE_WARNING,
	SPACE_LIMIT
};
class disk_container
{
public:
	disk_container(string _dir):
		m_dir(_dir),
            m_curr_disk_pos(-1),
		m_target_space(0),
		m_target_space_used(0),
		m_work_level(SPACE_NORMAL),
		m_ignore_warning(false),
		m_init_flag(false){}
	disk_container():
		m_target_space(0),
		m_target_space_used(0),
		m_init_flag(false){}
	void setdir(string dir, string brick, vector<unsigned int>& _pos);
	int init();
	int copy(const char *_srcfile, const char * _file);
	int create_dir(const char *_folder);
	bool prepare(){return m_init_flag;}
private:
	int copy_try(const char *_srcfile);
	int check();				// 根据BRICK_ID请求硬件部分将下一块磁盘挂在至制定处，路径通过BRICK_ID
	string m_dir;	// without dis
	vector<unsigned int> m_pos;  // the hard disk which in specify pos
   int m_curr_disk_pos;          // The current use of the hard disk 
    
	unsigned long long m_target_space;
	unsigned long long m_target_space_used;
	unsigned long long m_target_space_limit;
	
	string m_identity;			// BRICK_ID
	string m_curr_disk_id;		// DISK_ID
	_lev m_work_level;
	bool m_ignore_warning;
	bool m_init_flag;
	db_exec m_db;
};
#endif// _DISK_CONTAINER_H_
