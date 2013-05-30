//#include <io.h>
//#include <Tchar.h>
//#include <iostream>
//#include <windows.h>
//#include <psapi.h>
//#include <direct.h> // _rmdir
#include "file_frame.h"
//#include <time.h>
#include "systools.h"

using namespace std;



file_frame::file_frame(string _brick_id):
m_src_path(""),
m_DestinationPath(""),
total(0),
pprocess(NULL),
pflter(NULL),
pstatistic(NULL),
pshow(NULL),
pdata(NULL),
m_brick_id(_brick_id)
{
	strfind = "\\*.*";
	dis = DIS;
	//ElapsedTime = 0;
	startime = 0;
}

file_frame::~file_frame(void)
{
}

bool file_frame::init(void)
{
	if (pprocess == NULL ||
		pflter == NULL ||
		pstatistic == NULL || 
		pshow == NULL)
	{
		return false;
	}
	time ( &startime );
	return true;
}
int file_frame::search_by_folder(string path, string sourcepath)
{
	int resl = FRAME_CONTI;
	m_src_path = sourcepath;
#ifdef SYS_WINDOWS
	long handle;
	// 文件信息
	struct _finddata_t fileinfo;

	// 查找文件夹下文件
	handle = _findfirst(string(m_src_path + path + strfind).c_str(), &fileinfo);

	// 获得查找 句柄
	if (-1 == handle) 
	{
//		cout << "findfirst failed" << endl;
		return FRAME_ERR;
	}

	// 如果不是 ".",".."特殊目录	则打印正在处理的文件夹或文件
	if (strcmp(fileinfo.name, ".")
		&&strcmp(fileinfo.name, ".."))
	{
		//cout << fileinfo.name << endl;
	}

	if ((fileinfo.attrib & _A_SUBDIR) == _A_SUBDIR
		&&strcmp(fileinfo.name, ".")
		&&strcmp(fileinfo.name, ".."))
	{
		col.push_back(string(path + dis + fileinfo.name).c_str());
	}

	while(!_findnext(handle,&fileinfo))
	{
		if (strcmp(fileinfo.name, ".")
			&&strcmp(fileinfo.name, ".."))
		{
			//cout << fileinfo.name << endl;
			//cout << fileinfo.name << "\t";
		}

		if (!strcmp(fileinfo.name, ".")||!strcmp(fileinfo.name, ".."))
			continue;
		int ret = 0;
		if ((fileinfo.attrib & _A_SUBDIR) == _A_SUBDIR)
		{
			if (pflter(&fileinfo, (m_src_path + path + dis + fileinfo.name).c_str(), pdata) == FRAME_FILTER_IN)
			{
				ret = pprocess(m_src_path, m_DestinationPath, path + dis + fileinfo.name, true, pdata);
			}
			else if (pflter(&fileinfo, NULL, pdata) == FRAME_STOP)
			{
				resl = FRAME_STOP;
				break;
			}
			else
			{
				continue;
			}
			col.push_back(string(path + dis + fileinfo.name).c_str());
//			continue;
		}
		else
		{
			// 文件过滤
			if (pflter(&fileinfo, NULL, pdata) == FRAME_FILTER_IN)
			{
				// 打印处理的文件名
//				cout << "num "<< total << ": "<< fileinfo.name << endl;
				ret = pprocess(m_src_path, m_DestinationPath, path + dis + fileinfo.name, false, pdata);
			}
			else if (pflter(&fileinfo, NULL, pdata) == FRAME_STOP)
			{
				resl = FRAME_STOP;
				break;
			}
			else
			{
				continue;
			}
		}
		pstatistic(ret, pdata);
		total ++;
	}
	_findclose(handle);
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	//FILE* fp = NULL;
	DIR*  pDir = NULL;
	//fp = fopen((sourcepath + path).c_str(),"r");
	//if (fp!=NULL)
	//{
	//	fclose(fp);
	//	return resl;
	//	//return;
	//}
	pDir = opendir((sourcepath + path).c_str());
	if (pDir==NULL)
	{
//		cout << "opendir failed " << endl;
		return FRAME_ERR;
	}
	struct dirent *pDirent;
	int ret = 0;
	while((pDirent = readdir(pDir)) != NULL)
	{
		if (!strcmp(pDirent->d_name, ".")||!strcmp(pDirent->d_name, ".."))
			continue;
//		printf("d_type:%d,d_name: %s\n",pDirent->d_type,pDirent->d_name);
       // 文件过滤
        if (pflter(NULL, (m_src_path + path + dis + pDirent->d_name).c_str(), pdata) == FRAME_FILTER_IN)
		{
			// 打印处理的文件名
//			cout << "num "<< total << ": "<< pDirent->d_name << endl;
			ret = pprocess(m_src_path, m_DestinationPath, path + dis + pDirent->d_name, false, pdata);
		}
		else if (pflter(NULL, (m_src_path + path + dis + pDirent->d_name).c_str(), pdata) == FRAME_STOP)
		{
			resl = FRAME_STOP;
			break;
		}
		else
		{
			continue;
		}

        // on xfs, can't use readdir  d_type
        //if (pDirent->d_type == DT_REG)//普通文件
		//{
		//}
		//else if (pDirent->d_type == DT_DIR)
		//{
		//  col.push_back(string(path + dis + pDirent->d_name).c_str());
		//}
        if(isfolder((m_src_path + path + dis + pDirent->d_name).c_str()))
        {
			col.push_back(string(path + dis + pDirent->d_name).c_str());            
        }
		pstatistic(ret, pdata);
		total ++;

	}
	closedir(pDir);

#endif // SYS_LINUX
	return resl;
}
#ifdef SYS_WINDOWS
//void file_frame::set_opt(void *_p, _functype type)
//{
//	switch (type)
//	{
//	case process:
//		pprocess = static_cast<callback_proc>(_p);
//		break;
//	case statistic:
//		pstatistic = static_cast<callback_stat>(_p);
//		break;
//	case showstatistic:
//		pshow = static_cast<callback_show>(_p);
//		break;
//	case filter:
//		pflter = static_cast<callback_filter>(_p);
//		break;
//	case data:
//		pdata = _p;
//		break;
//	}
//}
#endif // SYS_WINDOWS
void file_frame::set_opt(_functype type, ... )
{
	va_list arg;
	va_start(arg, type);
	switch (type)
	{
	case process:
		pprocess = va_arg(arg, callback_proc);
		break;
	case statistic:
		pstatistic = va_arg(arg, callback_stat);
		break;
	case showstatistic:
		pshow = va_arg(arg, callback_show);
		break;
	case filter:
		pflter = va_arg(arg, callback_filter);
		break;
	case data:
		pdata = va_arg(arg, void* );
		break;
	}
}
void file_frame::add_dir(const char* src_path, const char* tar_path)
{
	if (src_path == NULL)
	{
		return;
	}
	pref tmp;
	tmp.m_src_path = src_path;
	if (tar_path != NULL)
	{
		tmp.m_tar_path = tar_path;
	}
	m_pref.push_back(tmp);
}
#ifdef SYS_WINDOWS
// false =  del self
//int file_frame::DeleteDir ( const char* pszDir, bool bDelSelf )
//{
//	if ( pszDir == NULL 
//		|| strlen ( pszDir ) == 0 ) 
//		return -1;
//	struct _finddata_t c_file;
//	//intptr_t hFile;
//	long hFile;
//	string strDir = pszDir;
//	size_t nLen = strDir.length ();
//	if ( strDir.at ( nLen - 1 ) == '\\' 
//		|| strDir.at ( nLen - 1 ) == '//')
//	{
//		strDir += "*.*";
//	}
//	else
//	{
//		strDir += "\\*.*";
//	}
//	if ( ( hFile = _findfirst ( strDir.c_str (), &c_file ) ) == -1L )
//	{
//		return  -1;
//	}
//	else
//	{
//		do 
//		{
//			if ( strcmp ( c_file.name, "." ) != 0 && strcmp ( c_file.name, ".." ) != 0 )
//			{ 
//				strDir = pszDir;
//				size_t nLen = strDir.length ();
//				if ( strDir.at ( nLen - 1 ) != '\\'
//					&& strDir.at ( nLen - 1 ) != '//' )
//				{
//					strDir += "\\";
//				}
//				strDir += c_file.name;
//				if ( c_file.attrib & _A_SUBDIR )			  
//				{
//					DeleteDir ( strDir.c_str () );
//					_rmdir ( strDir.c_str () );
//				}
//				remove ( strDir.c_str () );   
//			}
//		} while ( _findnext ( hFile, &c_file ) == 0 );
//
//		_findclose( hFile );
//	}
//	return  ( bDelSelf ) ?_rmdir ( pszDir ) : 0;
//}
#endif // SYS_WINDOWS
int file_frame::start()
{
//	cout << "-----------------------------" << endl;
//	cout << "------------start------------" << endl;

	for (list<pref>::iterator pref_item = m_pref.begin(); pref_item != m_pref.end(); ++pref_item)
	{

	//while (m_pref.size() != list<pref>::size_type(0))
	//{
	//	list<pref>::iterator pref_item = m_pref.begin();
		col.push_back("");
		// 处理
		list<string>::iterator item = col.begin();
		while(col.size() != list <int>::size_type(0))
		{
			list<string>::iterator item = col.begin();
			
			m_DestinationPath = (*pref_item).m_tar_path; // for other fun
			int ret = search_by_folder(*item, (*pref_item).m_src_path);
			if ( ret == FRAME_ERR)
			{
				LOG(WARNING) << "file_frame search error";
//				cout << (*pref_item).m_src_path + (*item) << " access failed !"<< endl;
			}
			else if (ret == FRAME_STOP)
			{
				return FRAME_STOP;
			}
			col.pop_front();
		}

	//	m_pref.pop_front();
	//}
	}


	return 0;
}

int file_frame::finish()
{
//	cout << "------------finish------------" << endl;
//	cout << "total :" << total << endl;
	total = 0;
	return pshow(pdata);
}
