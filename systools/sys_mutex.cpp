#include "sys_mutex.h"

sys_mutex::sys_mutex(void)
{
#ifdef SYS_WINDOWS
	m_hMutex = CreateMutex ( NULL, FALSE, NULL );
#endif //SYS_WINDOWS
}
sys_mutex::sys_mutex( char* lpszName )
{
#ifdef SYS_WINDOWS
	m_hMutex = CreateMutexA ( NULL, FALSE, lpszName );
	if ( m_hMutex == NULL )
	    m_hMutex = OpenMutexA ( MUTEX_ALL_ACCESS, FALSE, lpszName );
#endif //SYS_WINDOWS
}
sys_mutex::~sys_mutex(void)
{
#ifdef SYS_WINDOWS
	CloseHandle( m_hMutex );
#endif //SYS_WINDOWS
}
bool sys_mutex::lock ()
{
#ifdef SYS_WINDOWS
	if ( WAIT_FAILED == WaitForSingleObject( m_hMutex, INFINITE ) )
	{
		return false;
	} 
	return true;
#endif //SYS_WINDOWS
	return true;
}
bool sys_mutex::unlock ()
{
#ifdef SYS_WINDOWS
	if ( !ReleaseMutex ( m_hMutex ) ) 
	{
		return false;
	}
	return true;
#endif //SYS_WINDOWS
	return true;
}