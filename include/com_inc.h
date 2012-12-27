#ifndef _COM_INC_H_
#define _COM_INC_H_

#include "sys_define.h"

#ifdef SYS_WINDOWS
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <io.h>
#include <direct.h>
#include <tchar.h>
#define DIS  "\\"
#ifndef usleep
#define usleep(x) Sleep(x/1000)
#endif //usleep(x)

#endif // SYS_WINDOWS

#ifdef SYS_LINUX
#ifndef IN_WIN
#include <unistd.h>
#include <sys/stat.h> // mkdir
#include <sys/types.h>
#include <dirent.h>
#define DIS  "/"
#endif // IN_WIN
#include <pthread.h>
#include <semaphore.h>
#endif // SYS_LINUX
#include <glog/logging.h>
using google::InitGoogleLogging;
using google::SetLogDestination;
using google::FlushLogFiles;
using google::INFO;
using google::WARNING;
using google::FATAL;
//using google::ERROR; // define ERROR in windows.h
#include <time.h>
#include <cstring>

#include <string>
using std::string;

#endif //  _COM_INC_H_
