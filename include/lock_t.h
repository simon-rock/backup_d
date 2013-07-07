#ifndef _LOCK_T_H_
#define _LOCK_T_H_
#include "com_inc.h"
class lock_t
{
private:
#ifdef SYS_WINDOWS
	CRITICAL_SECTION m_critsec;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	pthread_mutex_t m_critsec;
#endif //SYS_LINUX
public:
	lock_t()
	{
		#ifdef SYS_WINDOWS
		::InitializeCriticalSection(&m_critsec);
		#endif // SYS_WINDOWS
#ifdef SYS_LINUX
		pthread_mutex_init(&m_critsec, NULL);
#endif //SYS_LINUX
	}

	~lock_t()
	{
#ifdef SYS_WINDOWS
		::DeleteCriticalSection(&m_critsec);
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
		pthread_mutex_destroy(&m_critsec);
#endif //SYS_LINUX
	}

	void Lock()
	{
		#ifdef SYS_WINDOWS
		::EnterCriticalSection(&m_critsec);
		#endif // SYS_WINDOWS
#ifdef SYS_LINUX
		pthread_mutex_lock(&m_critsec);
#endif //SYS_LINUX
	}

	void Unlock()
	{
		#ifdef SYS_WINDOWS
		::LeaveCriticalSection(&m_critsec);
		#endif // SYS_WINDOWS
#ifdef SYS_LINUX
		pthread_mutex_unlock(&m_critsec);
#endif //SYS_LINUX
	}
protected:
};

#endif // _LOCK_T_H_
