//	msg_queue.h
#ifndef _MSG_QUEUE_H
#define _MSG_QUEUE_H
#include "com_inc.h"
#include "backup_msg.h"
#include "lock.h"
#include <list>
using std::list;
typedef struct _msg
{
	int msg_num;
	void* para;
	int para_length;
}msg;
class msg_queue
{
public:
	static msg_queue* instance()
	{
		static msg_queue ins;
		return &ins;
	}
	~msg_queue();
	void push_back(msg *tmp);
	msg* pop_front();
	msg* front();
	void clear();
	int size();
	bool wait_msg();

private:
	list<msg*> m_queue;
	lock_obj m_mutex;
	//lock::mutex_type m_mutex;	// #include "lock.h"
	//sys_mutex m_mutex;		// #include "sys_mutex.h"
#ifdef SYS_WINDOWS
	HANDLE m_htask; 
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	sem_t m_htask;
#endif // SYS_LINUX
	msg_queue(void);
	void set_empty();
	void set_task();
};

// _x	: 消息类型
// _p	: 消息参数 (void*) 类型 (注消息参数中不能含有指针)
// _pl	: 消息参数长度
#define POSTMESSAGEORA(_m, _p, _pl) {msg* m = new msg();m->msg_num = _m;m->para = _p;m->para_length = _pl;msg_queue::instance()->push_back(m);}
#define POSTMESSAGENODATA(_type) {msg* m = new msg();m->msg_num = MSG(_type);m->para = NULL;m->para_length = 0;msg_queue::instance()->push_back(m);}
#define POSTMESSAGE(_type, _p) {msg* m = new msg();m->msg_num = MSG(_type);m->para = _p;if(_p == NULL)m->para_length = 0; else m->para_length = sizeof(MSG_DATA(_type));msg_queue::instance()->push_back(m);}

#define DEFINEMSGDATA(_pd, _data_type) \
	MSG_DATA(_data_type) *_pd= new MSG_DATA(_data_type); \
	memset(_pd,0,sizeof(MSG_DATA(_data_type)));

#define COPYDATA(_pd_tar, _pd_src, _l) \
	strncpy(_pd_tar, _pd_src, _l < sizeof(_pd_tar) ? _l : sizeof(_pd_tar) - 1);
#endif // _TASK_QUEUE_H