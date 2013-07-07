#include "sys_thread.h"
sys_thread::sys_thread():
#ifdef SYS_WINDOWS
m_hthread(NULL),
m_dwexit(0),
#endif // SYS_WINDOWS
m_ptask(NULL),

m_brunning(false),
m_bpaused(true),			// stop thread when sys start
m_uithreadID(0)
{
	/*
#ifdef SYS_WINDOWS
	m_hthread = NULL;
	m_dwexit = 0;
#endif // SYS_WINDOWS
	*/
}

sys_thread::~sys_thread()
{
	end();
}

void sys_thread::set_task(task_t* task)
{
	m_ptask = task;
}

void sys_thread::begin()
{
/*#if defined( _WIN32 )*/
#ifdef SYS_WINDOWS
	// Start the thread.
	m_hthread = (HANDLE)_beginthreadex( NULL,
		0,
		&cThreadProc,
		(void*)this,
		0,
		&m_uithreadID );

	m_brunning = true;
	if( m_hthread == NULL )
	{
		// add extra error-handling here.
		m_brunning = false;
	}
#endif // SYS_WINDOWS /* defined( _WIN32 ) */
#ifdef SYS_LINUX
	int ret = pthread_create(&m_hthread, 0, cThreadProc, (void*)this);
	m_brunning = true;
	if( ret == -1 )
	{
		// You can add extra error-handling here.
		m_brunning = false;
	}
#endif // SYS_LINUX
}

void sys_thread::end()
{
/*#if defined( _WIN32 )*/
#ifdef SYS_WINDOWS
	if( m_hthread != NULL )
	{
		m_brunning = false;
		WaitForSingleObject( m_hthread, INFINITE );			// 等待自动退出
		DWORD ExitCode;
		GetExitCodeThread( m_hthread, &ExitCode );
		m_dwexit = ExitCode;
		CloseHandle( m_hthread );
		m_hthread = NULL;
	}
#endif // SYS_WINDOWS/* defined( _WIN32 ) */
#ifdef SYS_LINUX
		m_brunning = false;
		pthread_join(m_hthread, NULL);
#endif // SYS_LINUX

}

int sys_thread::ThreadProc()
{
//	m_ptask = NULL;  // 创建线程运行 set_task
	int i = 0;

	while(m_brunning)
	{
		usleep(100*1000);				// wait some time, 
		if(!m_bpaused)
		{
			m_lock.Lock();				// no task, m_ptask->get_data() return null
			{
//				printf("[get task][%ud][%ud]\n", m_uithreadID, m_ptask);			// for bug : m_ptask = NULL
				task_fun task = m_ptask->get_task();
				task_data* data = m_ptask->get_data();
				// Run the actual task
				task(data);
				// Task is complete.
				m_bpaused = true;
				delete m_ptask;
				m_ptask = NULL;
			}
			m_lock.Unlock();
		}
	}


	if(m_ptask)
	{
		delete m_ptask;
		m_ptask = NULL;
	}

	return 0;
}
