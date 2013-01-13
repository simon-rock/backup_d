// SVG_MSGQueue.cpp
#include "msg_queue.h"
//#include "SVG_MSG.h"

msg_queue::msg_queue(void)
{
#ifdef SYS_WINDOWS
	m_htask = CreateEvent(NULL, TRUE, FALSE, _T("have_msg"));
	//m_mutex = lock::create_mutex();
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	sem_init(&m_htask, 0, 0);
#endif // SYS_LINUX
	
}

msg_queue::~msg_queue()
{
#ifdef SYS_WINDOWS
	CloseHandle(m_htask);
	//lock::destory_mutex(&m_mutex);
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	sem_destroy(&m_htask);
#endif // SYS_LINUX
}

void msg_queue::push_back(msg *tmp)
{
	lock l(&m_mutex);
	m_queue.push_back(tmp);
	set_task();
}

msg*  msg_queue::pop_front()
{
	lock l(&m_mutex);
	msg* tmp = front();
	//
	m_queue.pop_front();
	if (size() == 0 )
	{
		set_empty();
	}
	return tmp;
}
msg* msg_queue::front()
{
	lock l(&m_mutex);
	msg* tmp;
	if (m_queue.size() > 0)
	{
		tmp =  m_queue.front();
	}
	else
	{
		tmp = 0;
	}
	return tmp;
}
void msg_queue::clear()
{
	lock l(&m_mutex);
	m_queue.clear();
	while (m_queue.size()>0)
	{
		delete(m_queue.front());
		m_queue.pop_front();
	}
	set_empty();
}
int msg_queue::size()
{
	lock l(&m_mutex);
	int size =  m_queue.size();
	return size;
}
bool msg_queue::wait_msg()
{
#ifdef SYS_WINDOWS

	HANDLE task = CreateEvent(NULL, TRUE, FALSE, _T("have_msg"));
	if (NULL == task)
	{
		return false;
	}
	DWORD dwRet = WaitForSingleObject( task, INFINITE );
	if ( dwRet == WAIT_OBJECT_0 )
	{
		return true;
	}
	else
	{
		return false;
	}
#endif //SYS_WINDOWS
#ifdef SYS_LINUX
	int ret = sem_wait(&m_htask);
	if ( 0 != ret )
	{
//		cout<<"CEventImpl::WaitImpl sem_wait failed"<<endl; 
	}
	sem_post(&m_htask);
	return true;
#endif //SYS_LINUX
}

void msg_queue::set_task()
{
#ifdef SYS_WINDOWS
	SetEvent(m_htask);
	return;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	//if (sem_trywait(&m_htask) == EAGAIN)
	//{
	int ret = sem_post(&m_htask);
	if ( 0 != ret )
	{
//		cout<<"cannot signal event"<<endl;
	}
	//}
#endif //SYS_LINUX
}
void msg_queue::set_empty()
{
#ifdef SYS_WINDOWS
	ResetEvent (m_htask);
	return;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	int sval = 0;
	do 
	{
		sem_trywait(&m_htask);
		sem_getvalue(&m_htask, &sval);
	} while(sval > 0);
#endif //SYS_LINUX
}