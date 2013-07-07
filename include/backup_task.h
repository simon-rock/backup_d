#ifndef _BACKUP_TASK_H_
#define _BACKUP_TASK_H_
#include "com_inc.h"
#include "task_t.h"
#include "lock.h"
#include <list>
#include <vector>
#include <string>
using std::list;
using std::vector;
using std::string;
class backup_task : public task_data
{
public:
	backup_task():last_filter_time(0), last_filter_interval(0){}
	~backup_task(){}
	string brick_id;                // brick id
	string mount_path;              // mount brick to local path
	string backup_path;             // backup to local path
	long long last_filter_time;     // last backup time
	long long last_filter_interval; // distance from current time

	// 硬件用
	string brick_path;              // brick new path
	vector<unsigned int> pos;       // sp. disk

	//
	//thread_id;
	//thread_stop;
	//

	backup_task(backup_task& _o)
	{
		brick_id = _o.brick_id;
		mount_path = _o.mount_path;
		backup_path = _o.backup_path;
		last_filter_time = _o.last_filter_time;
		last_filter_interval = _o.last_filter_interval;

		// 硬件用
		brick_path = _o.brick_path;
		pos = _o.pos;
	}
private:
	backup_task operator = (backup_task&);
};

// func
void proc(task_data* data);


class task_control
{
public:
	static task_control *Instance(){
		static task_control instance;
		return &instance;
	}
	~task_control(void){}


	bool reg(const string& _brick_id);
	void unreg(const string& _brick_id);
	void set_sem(const string& _brick_id);
	bool wait_sem(const string& _brick_id, bool infinite = false);
	void set_all();
private:
	typedef struct _task_sem
	{
		string brick_id;
#ifdef SYS_WINDOWS
		HANDLE sem;				// stop task
#endif //SYS_WINDOWS
#ifdef SYS_LINUX
		sem_t sem;
#endif // SYS_LINUX
	}task_sem;
	list<task_sem*> _v;
	lock_obj m_lock;
	task_control(void){}
	task_control(const task_control&);
	task_control& operator = (const task_control&);
};
#endif // _BACKUP_TASK_H_