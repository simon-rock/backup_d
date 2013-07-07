#ifndef _LOCK_H__ 
#define _LOCK_H__ 

#include "com_inc.h"

class lock;
class lock_obj
{
	friend class lock;
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
	HANDLE _s;
#else
	CRITICAL_SECTION _s;
#endif // MUTEX_WINDOWS 
#endif // SYS_WINDOWS 
#ifdef SYS_LINUX
	pthread_mutex_t m_critsec;
#endif //SYS_LINUX
	void lock()
	{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
		if ( WAIT_FAILED == WaitForSingleObject( _s, INFINITE ) )
		{
			// error
		} 
#else
		::EnterCriticalSection( & _s);
#endif // MUTEX_WINDOWS 
#endif // SYS_WINDOWS 
#ifdef SYS_LINUX
		pthread_mutex_lock(&m_critsec);
#endif //SYS_LINUX		
	} 
	void unlock()
	{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
		if ( !ReleaseMutex ( _s ) ) 
		{
			// error
		}
#else
		::LeaveCriticalSection( & _s);
#endif // MUTEX_WINDOWS 
#endif // SYS_WINDOWS 
#ifdef SYS_LINUX
		pthread_mutex_unlock(&m_critsec);
#endif //SYS_LINUX
	} 
public:
	lock_obj()
	{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
		_s = CreateMutex ( NULL, FALSE, NULL );
#else
		::InitializeCriticalSection( & _s);
#endif // MUTEX_WINDOWS 
#endif // SYS_WINDOWS 
#ifdef SYS_LINUX
		pthread_mutexattr_t p_attr;
		pthread_mutexattr_init(&p_attr);
		pthread_mutexattr_settype(&p_attr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&m_critsec, &p_attr);
		pthread_mutexattr_destroy(&p_attr);
#endif //SYS_LINUX
		
	} 
	~ lock_obj()
	{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
		CloseHandle( _s );
#else
		::DeleteCriticalSection( & _s);
#endif // MUTEX_WINDOWS 
#endif // SYS_WINDOWS 	
#ifdef SYS_LINUX
		pthread_mutex_destroy(&m_critsec);
#endif //SYS_LINUX
	} 
} ;

class lock 
{
public:
	explicit lock(lock_obj* _s)
	{
		m_mutex = _s;
		if (NULL != m_mutex)
			m_mutex -> lock();
	}
	~lock()
	{
		if (NULL != m_mutex)
			m_mutex -> unlock(); 
	}
protected:
private:
	lock(){}
	lock_obj* m_mutex;
};
#endif // _LOCK_H__ 
/*
class lock 
{
public:
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
	typedef HANDLE mutex_type;
#else
	typedef InstanceLockBase  mutex_type;
#endif //MUTEX_WINDOWS 
#endif //SYS_WINDOWS
	explicit lock(mutex_type* _s);
	~lock();
	static mutex_type create_mutex();
	static void destory_mutex(mutex_type* _s);
protected:
private:
	lock(){}
	mutex_type* m_mutex;
};
*/
