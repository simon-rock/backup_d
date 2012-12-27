#ifndef _SYS_MUTEX_H__
#define _SYS_MUTEX_H__

#include "com_inc.h"
//#include <windows.h>
class sys_mutex
{
public:
	sys_mutex(void);
	sys_mutex( char* lpszName );
	virtual ~sys_mutex(void);

	bool lock ();
	bool unlock ();
private:
#ifdef SYS_WINDOWS
	typedef HANDLE mutex_type;
	mutex_type m_hMutex;
#endif //SYS_WINDOWS

};

#endif //_SYS_MUTEX_H__