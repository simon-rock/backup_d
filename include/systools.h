#ifndef _SYS_TOOLS_H_
#define _SYS_TOOLS_H_

#include "com_inc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#ifdef SYS_LINUX
	void itoa ( unsigned long val, char *buf, unsigned radix );
	#define MKDIR(path) mkdir(path,0777)
	#define CHMOD(_src, _tar) \
	{	\
		struct stat64 buf;	\
		if (stat64(_src, &buf) == 0)	\
		{	\
			chmod(_tar, buf.st_mode);	\
		}	\
	}	
#endif // SYS_LINUX

#ifdef SYS_WINDOWS
#ifndef usleep
#define usleep(x) Sleep(x/1000)
#endif //usleep(x)
#define MKDIR(path) mkdir(path)
#define CHMOD(_src, _tar) 
#endif // SYS_WINDOWS

int getdiskspaceinfo(const char *dir, unsigned long long& freebycall, unsigned long long& total, unsigned long long& free);

long long getfilesize(const char *dir);
long long getfile_mtime(const char *dir);
int isfolder(const char *dir);
#endif // _SYS_TOOLS_H_