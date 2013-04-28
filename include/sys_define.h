
#ifndef _SYS_DEFINE_H_
#define _SYS_DEFINE_H_
//#define IN_WIN
#ifndef WIN32
#define SYS_LINUX
#ifndef __USE_FILE_OFFSET64
#define __USE_FILE_OFFSET64
#endif

#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#else
#define SYS_WINDOWS
//#define MUTEX_WINDOWS
#pragma warning(disable : 4996)
#endif // WIN32


#define DISK_USED_LINE		0.95		// 使用大于95%时报警
#define DISK_USED_LIMIT		0.50		// 目标目录可用空间不足50% 时报警
#define FIRSTBACKUPSPACEBEFCURR		60*60*24	// 首次备份结束时间，距当前
#define BACKUPSPACE			60*60*10	// 备份跨度
#define MINBACKUPSPACE		60*60*1		// 最小备份跨度
//		 glog
//#define GOOGLE_STRIP_LOG 1
//#define GLOG_NO_ABBREVIATED_SEVERITIES
//#define GOOGLE_GLOG_DLL_DECL=
#define GOOGLE_GLOG_DLL_DECL
#ifdef SYS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define  NOGDI
#endif //SYS_WINDOWS

#define VERSION "1.1"
#define FIFOVIEW "/tmp/backupview"
#define FIFOCONTROL "/tmp/backupcontrol"
#define CONFIGFILE "backup_config"        // backup config file
// error define 
#define BK_SUCESS				0x00000000
#define BK_STOP					0x00000001
#define BK_ERROR				0xffffffff

#define BK_INIT_ERR				0x10000001
#define BK_INIT_NEED_DEL		0x10000002
#define BK_INIT_NEEDED			0x10000003
#define BK_INIT_NO_DISK_MOUNT	0x10000004
#define BK_INIT_ADD_DISK		0x10000005
#define BK_INIT_SRC_CREATE_ER		0x10000006
#define BK_INIT_SRC_MOUNT_ER		0x10000007
#define BK_INIT_SRC_EXIST		0x10000008

#define BK_BACKUP_FOLDER_ERR	0x20000001
#define BK_BACKUP_FILE_ERR		0x20000002
#define BK_BACKUP_SPACE_LIMIT	0x20000003
#define BK_BACKUP_SPACE_WARNING	0x20000004
#define BK_SUCESS_FILE			0x20000005
#define BK_SUCESS_FOLDER		0x20000006

#define BK_DB_SUCESS			0x30000001
#define BK_DB_ERROR				0x30000002
#define BK_DB_EXIST				0x30000003
#define BK_DB_NOEXIST			0x30000004
#endif // _SYS_DEFINE_H_


