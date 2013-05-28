#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

// #define WINDOWS_LEAN_AND_MEAN

#include<iostream>
using std::cout;

#include<queue>
#include<vector>
using std::queue;
using std::vector;

#include"task_t.h"
#include"sys_thread.h"

class thread_pool
{	
private:
	queue<task_t*>		m_qtaskList;			// 
	vector<sys_thread*>	m_vthreads;				//
	unsigned int		m_nactive;				// 活动现成数
	unsigned int		m_ndormant;				// 等待任务的现成数

	static thread_pool m_instance;
	thread_pool()
	{
	}
	thread_pool(const thread_pool&);
	thread_pool& operator = (const thread_pool&);
	~thread_pool()
	{
		shut_down();
	}
public:

	static thread_pool* getInstance()   
	{	
		return &m_instance;			
	}
	unsigned int  getActiveThreads()	 
	{	
		return m_nactive;			
	}
	unsigned int  getNumDormantThreads() 
	{	
		return m_ndormant;			
	}
	size_t  getNumQueuedTasks()			 
	{	
		return m_qtaskList.size();	
	}

	void initialize(unsigned int uiTryMinNumThreads,
					unsigned int uiTryMaxNumThreads);

	void shut_down();
	void add_task(task_fun task, task_data* data);

	void process();							// 给线程设置任务

protected:
};

#endif //_THREADPOOL_H_