#ifndef _THREAD_H_
#define _THREAD_H_

// #define WINDOWS_LEAN_AND_MEAN
#include "com_inc.h"

#include<iostream>
using std::cout;

#include"task_t.h"
#include"lock_t.h"

class sys_thread
{
private:
#ifdef SYS_WINDOWS
	HANDLE	m_hthread;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	pthread_t	m_hthread;
#endif // SYS_LINUX
	lock_t	m_lock;					// 当设置任务时 先锁住线程，然后设置任务，不过此处当没有任务是，线程会死循环
	task_t*	m_ptask;

	bool	m_brunning;
	bool	m_bpaused;				// 当线程被锁住设置任务后，置标志，开启线程，完成任务后线程会重置标志停止线程
	unsigned int	m_uithreadID;	// 现成ID
#ifdef SYS_WINDOWS
	DWORD	m_dwexit;
#endif // SYS_WINDOWS

public:
	bool	isRunning()	{	return m_brunning;	}
	bool	isPaused()	{	return m_bpaused;	}
	unsigned int	getThreadID(){	return m_uithreadID;	}
#ifdef SYS_WINDOWS
	DWORD	getExitCode(){	return m_dwexit;}
#endif // SYS_WINDOWS
	
	void	Lock()	{	m_lock.Lock();	}
	void	Unlock(){	m_lock.Unlock();}

	void	Pause()	{	m_bpaused = true;	}
	void	Resume(){	m_bpaused = false;	}

	sys_thread();
	~sys_thread();
	void set_task(task_t* task);
	void begin();
	void end();
	int ThreadProc();
protected:
#ifdef SYS_WINDOWS
	static unsigned __stdcall cThreadProc(void* _pThis)
	{
		return (static_cast<sys_thread*>(_pThis))->ThreadProc();
	}
#endif //SYS_WINDOWS
#ifdef SYS_LINUX
	static void * cThreadProc(void* _pThis)
	{
		return (void*)((static_cast<sys_thread*>(_pThis))->ThreadProc());
	}
#endif //SYS_LINUX
};
#endif //_THREAD_H_