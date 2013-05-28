#pragma once

#include "com_inc.h"
#include <stdarg.h>
#include <list>
using std::list;

//#include <Windows.h>

//typedef   struct _finddata_t FILEINFO;

#define FRAME_FILTER_IN		0x10000001
#define FRAME_FILTER_E		0x10000002
#define FRAME_STOP			0x00000003
#define FRAME_CONTI			0x00000004
#define FRAME_ERR			-1

typedef int ( *callback_proc ) ( string SourcePath, string DestinationPath, string path, bool folder ,void *pdata);
typedef int ( *callback_stat ) ( int , void *pdata);
typedef int ( *callback_show ) (void *pdata);
typedef int ( *callback_filter ) (struct _finddata_t*  pfileinfo, const char * filepath, void *pdata);

class file_frame
{
public:
	file_frame(string _brick_id);
	~file_frame(void);

	enum _functype
	{
		process = 0,
		filter,
		statistic,
		showstatistic,
		data
	};
	//void set_opt(void *pfunc, _functype type);
	void set_opt(_functype type, ... );
	void add_dir(const char* src_path, const char* tar_path = NULL);
	bool init();
#ifdef SYS_WINDOWS
//	static int DeleteDir ( const char* pszDir, bool bDelSelf = false );// false 为不删除自己//与surdoc不同
#endif // SYS_WINDOWS
	int start();
	int finish();
#ifdef SYS_WINDOWS
//	__int64 GetSleepTime(){return m_SleepTime;}
#endif // SYS_WINDOWS
private:
	int search_by_folder(string path, string sourcepath); // 处理目标 文件夹

	typedef struct _pref
	{
		string m_src_path;
		string m_tar_path;
#ifdef SYS_WINDOWS
//		__int64 ElapsedTime;
#endif // SYS_WINDOWS
	}pref;
	string m_src_path;
	string m_DestinationPath;
	string strfind;
	string dis;
	list<string> col; // 待处理文件夹路径队列，相对源文件根目录或出力目录根目录
	int total;			//处理的文件数
	callback_proc pprocess;		// 处理的文件函数
	callback_filter pflter;		// 过滤函数
	callback_stat pstatistic;	// 统计函数
	callback_show pshow;		// 显示
	void *pdata;
	time_t startime;	 

	list<pref> m_pref;

	string m_brick_id;	// 
};
