//#include "lock.h"

/*
lock::lock(mutex_type* _s)
{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
	WaitForSingleObject( *_s, INFINITE );
#else
	::EnterCriticalSection(&_s);
	cout << "EnterCriticalSection(&_s);" << endl;
#endif //MUTEX_WINDOWS 
#endif //SYS_WINDOWS
	m_mutex = _s;
}

lock::~lock()
{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
	ReleaseMutex ( *m_mutex );
#else
	cout << "EnterCriticalSection(&_s);" << endl;
	::LeaveCriticalSection(&m_mutex);
#endif //MUTEX_WINDOWS 
#endif //SYS_WINDOWS
}

lock::mutex_type lock::create_mutex()
{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
	return CreateMutex ( NULL, FALSE, NULL );
#else
	CRITICAL_SECTION critsec;
	::InitializeCriticalSection(&critsec);
	return critsec;
#endif //MUTEX_WINDOWS 
#endif //SYS_WINDOWS
}

void lock::destory_mutex(mutex_type* _s)
{
#ifdef SYS_WINDOWS
#ifdef MUTEX_WINDOWS 
		CloseHandle( *_s );
#else
	::DeleteCriticalSection(&_s);
#endif //MUTEX_WINDOWS 
#endif //SYS_WINDOWS
}
*/