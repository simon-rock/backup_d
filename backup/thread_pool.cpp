#include "thread_pool.h"

thread_pool thread_pool::m_instance;

void thread_pool::initialize(unsigned int uiTryMinNumThreads,
				unsigned int uiTryMaxNumThreads)
{
	unsigned int i;

	m_vthreads.assign(uiTryMinNumThreads, NULL);

	for(i = 0; i < uiTryMinNumThreads; ++i)
	{
		m_vthreads[i] = new sys_thread();
		m_vthreads[i]->begin();
		if(m_vthreads[i]->isRunning())
		{
			++m_nactive;
		}
	}

	for(i = uiTryMinNumThreads; i < uiTryMaxNumThreads; ++i)
	{
		sys_thread* trythread = new sys_thread();
		trythread->begin();
		if(trythread->isRunning())
		{
			m_vthreads.push_back(trythread);
			++m_nactive;
		}
		else
		{
			delete trythread;
			break;
		}
	}
}

void thread_pool::shut_down()
{
	task_t* deltask;
	sys_thread* delthread;
	//Clear the tasks
	while(!m_qtaskList.empty())
	{
		deltask = m_qtaskList.front();
		m_qtaskList.pop();
		delete deltask;
	}

	//Clear the threads
	while(!m_vthreads.empty())
	{
		delthread = m_vthreads.back();
		m_vthreads.pop_back();
		delthread->end();
		delete delthread;
	}
}

void thread_pool::add_task(task_fun task, task_data* data)
{
	task_t* newTask = new task_t(task,data);
	m_qtaskList.push(newTask);
}

void thread_pool::process()
{
	vector<sys_thread*> pDormantThreads;
	pDormantThreads.assign(m_vthreads.size(), NULL);
	unsigned int d = 0, dsize = 0, i = 0;
	for(i = 0; i < m_vthreads.size(); ++i)
	{
		if(m_vthreads[i]->isRunning() && m_vthreads[i]->isPaused())   // 可运行，并等待任务的
		{
			pDormantThreads[d] = m_vthreads[i];
			++d;
		}
	}
	dsize = d;

	//Now, if we have tasks, assign them to the dormant threads.
	//Otherwise, break.
	for(i = 0; i < dsize; ++i)
	{
		if(m_qtaskList.size() > 0)
		{
			task_t* nextTask = m_qtaskList.front();
			m_qtaskList.pop();
			pDormantThreads[i]->Lock();       // 布置任务是锁住线程
			pDormantThreads[i]->set_task(nextTask);
			pDormantThreads[i]->Resume();
			pDormantThreads[i]->Unlock();
			--d;
		}
		else
			break;
	}

	m_ndormant = d;
	pDormantThreads.clear();

}