#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "file_frame.h"
#include "systools.h"
//////////////////////makefile ����--del SYS_WINDOWS////////////////////////////////////////////////////
int _proc(string SourcePath, string DestinationPath, string path, bool folder, void* pdata)
{
	string src_path = (SourcePath + path).c_str();
	string tar_path = (DestinationPath + path).c_str();
	if (folder)
	{
		mkdir(tar_path.c_str());
		return 0;
	}
	char buf[1024] = {0};
	string sbuf;
	bool bskip = false;
	bool bfirst = true;
	ifstream   is; 
	ofstream   os;
	is.exceptions(ios_base::badbit);
	os.exceptions(ios_base::badbit);

	is.open( src_path.c_str(),ios::in | ios::binary); 
	os.open( tar_path.c_str(), ios::out|ios::binary); 
	if (!is||!os) 
	{
		cout << "[" << src_path << "]" << "error" << endl;
		return -1;
	}
	try   { 
		//os   <<   is.rdbuf();
		while(!is.eof())			// do with by line
		{
			is.getline(buf,1024);
			if (string(buf).find("#ifdef") != string::npos &&
				string(buf).find("SYS_WINDOWS") != string::npos)
			{
				bskip = true;
			}
			if (!bskip)
			{
				if (bfirst) 
					bfirst = false;
				else 
					os << endl;
				os<<buf;
			}
			else if (string(buf).find("#endif") != string::npos &&
				string(buf).find("SYS_WINDOWS") != string::npos)
			{
				bskip = false;
			}
		}
	} 
	catch   (ifstream::failure   e)   { 
		os.flush();
		os.close();
		is.close();
		remove(tar_path.c_str());
		cout << "[" << src_path << "]" << "error" << endl;
		return -1;
	} 
	os.flush();
	os.close();
	is.close();
	return 0;
}

int _statistic ( int ret, void* pdata)
{
	return 0;
}
int _show (void* pdata)
{
	return 0;
}
int _filter (struct _finddata_t*  pfileinfo, const char * file_path, void* pdata)
{
	if (pfileinfo != NULL)
	{
		if ((pfileinfo->attrib & _A_SUBDIR) == _A_SUBDIR&&
			string(pfileinfo->name) != "Pre-built.2" &&
			string(pfileinfo->name) != "Debug" &&
			string(pfileinfo->name) != "Release" &&
			string(pfileinfo->name) != "glog-0.3.2" &&
			string(pfileinfo->name) != "glog_project" &&
			string(pfileinfo->name) != "test" &&
			string(pfileinfo->name) != "sql_wrapper" )
		{
			return FRAME_FILTER_IN;
		}

		char   ext[50]; 
		_splitpath(pfileinfo->name, NULL, NULL, NULL, ext);
		if (string(ext) == ".cpp" ||
			string(ext) == ".h" )
		{
			return FRAME_FILTER_IN;
		}
	}
	/*else if (file_path != NULL)
	{
		if (isfolder(file_path))
		{
			return FRAME_FILTER_IN;
		}
		long long mtime = getfile_mtime(file_path);
		LOG(INFO) << "[file_path]" << file_path << "[mtime]" << mtime << "[time_apce]" << p->m_lastbackup_time << " - " << p->m_end_time;
		if (mtime >= p->m_lastbackup_time 
			&& mtime < p->m_end_time )
		{
			LOG(INFO) << "[file_path]" << file_path << "[FRAME_FILTER_IN]";
			return FRAME_FILTER_IN;
		}
	}*/
	return FRAME_FILTER_E;
}

//////////////////////makefile ����--copy////////////////////////////////////////////////////
int _proc2(string SourcePath, string DestinationPath, string path, bool folder, void* pdata)
{
	string src_path = (SourcePath + path).c_str();
	string tar_path = (DestinationPath + path).c_str();
	if (folder)
	{
		mkdir(tar_path.c_str());
		return 0;
	}
	char buf[1024] = {0};
	string sbuf;
	bool bskip = false;
	ifstream   is; 
	ofstream   os;
	is.exceptions(ios_base::badbit);
	os.exceptions(ios_base::badbit);

	is.open( src_path.c_str(),ios::in | ios::binary); 
	os.open( tar_path.c_str(), ios::out|ios::binary); 
	if (!is||!os) 
	{
		cout << "[" << src_path << "]" << "error" << endl;
		return -1;
	}
	try   { 
		os   <<   is.rdbuf();	// copy
		//while(!is.eof())
		//{
		//	is.getline(buf,1024);
		//	if (string(buf).find("#ifdef") != string::npos &&
		//		string(buf).find("SYS_WINDOWS") != string::npos)
		//	{
		//		bskip = true;
		//	}
		//	if (!bskip)
		//	{
		//		os<<buf<<endl;
		//	}
		//	else if (string(buf).find("#endif") != string::npos &&
		//		string(buf).find("SYS_WINDOWS") != string::npos)
		//	{
		//		bskip = false;
		//	}
		//}
	} 
	catch   (ifstream::failure   e)   { 
		os.flush();
		os.close();
		is.close();
		remove(tar_path.c_str());
		cout << "[" << src_path << "]" << "error" << endl;
		return -1;
	} 
	os.flush();
	os.close();
	is.close();
	return 0;
}

int _filter2 (struct _finddata_t*  pfileinfo, const char * file_path, void* pdata)
{
	if (pfileinfo != NULL)
	{
		if ((pfileinfo->attrib & _A_SUBDIR) == _A_SUBDIR &&
			string(pfileinfo->name) != "Pre-built.2" &&
			string(pfileinfo->name) != "Debug" &&
			string(pfileinfo->name) != "Release" &&
			string(pfileinfo->name) != "glog-0.3.2" &&
			string(pfileinfo->name) != "glog_project" &&
			string(pfileinfo->name) != "test" &&
			string(pfileinfo->name) != "sql_wrapper" )
		{
			return FRAME_FILTER_IN;
		}
		char   ext[50]; 
		_splitpath(pfileinfo->name, NULL, NULL, NULL, ext);
		if (string(pfileinfo->name) == "Makefile" ||
			string(pfileinfo->name) == "Makefile_test" )
		{
			return FRAME_FILTER_IN;
		}
	}
	/*else if (file_path != NULL)
	{
		if (isfolder(file_path))
		{
			return FRAME_FILTER_IN;
		}
		long long mtime = getfile_mtime(file_path);
		LOG(INFO) << "[file_path]" << file_path << "[mtime]" << mtime << "[time_apce]" << p->m_lastbackup_time << " - " << p->m_end_time;
		if (mtime >= p->m_lastbackup_time 
			&& mtime < p->m_end_time )
		{
			LOG(INFO) << "[file_path]" << file_path << "[FRAME_FILTER_IN]";
			return FRAME_FILTER_IN;
		}
	}*/
	return FRAME_FILTER_E;

}
//////////////////////src ����--del SYS_LINUX////////////////////////////////////////////////////
int _proc3(string SourcePath, string DestinationPath, string path, bool folder, void* pdata)
{
	string src_path = (SourcePath + path).c_str();
	string tar_path = (DestinationPath + path).c_str();
	if (folder)
	{
		mkdir(tar_path.c_str());
		return 0;
	}
	char buf[1024] = {0};
	string sbuf;
	bool bfirst = true;
	ifstream   is; 
	ofstream   os;
	is.exceptions(ios_base::badbit);
	os.exceptions(ios_base::badbit);

	is.open( src_path.c_str(),ios::in | ios::binary); 
	os.open( tar_path.c_str(), ios::out|ios::binary); 
	if (!is||!os) 
	{
		cout << "[" << src_path << "]" << "error" << endl;
		return -1;
	}
	try   { 
		//os   <<   is.rdbuf();
		while(!is.eof())
		{
			is.getline(buf,1024);
			if ((string(buf).find("#ifdef") != string::npos &&
				string(buf).find("SYS_LINUX") != string::npos) ||
				(string(buf).find("#endif") != string::npos &&
				string(buf).find("SYS_LINUX") != string::npos))
			{
			}
			else
			{
				if (bfirst) 
					bfirst = false;
				else 
					os << endl;
				os<<buf;
			}
		}
	} 
	catch   (ifstream::failure   e)   { 
		os.flush();
		os.close();
		is.close();
		remove(tar_path.c_str());
		cout << "[" << src_path << "]" << "error" << endl;
		return -1;
	} 
	os.flush();
	os.close();
	is.close();
	return 0;
}

int main(int argc, char ** argv)
{
	if (argc < 2)
	{
		cout << "no input folder" << endl;
		return 0;
	}
	//string src_path = "E:\\resource\\backup_server\\test\\backup";
	string src_path = argv[1];

	string tar_path = src_path  + "_format";
	string rel_path = src_path  + "_release";
	// perpare
	if(access(tar_path.c_str(),0)!=0)//access�����ǲ鿴�ļ��ǲ��Ǵ���
	{
		if (MKDIR(tar_path .c_str()))//��������ھ���mkdir����������
		{
			cout << "create folder[" << tar_path.c_str() <<"]" << " error" << endl;
			return 0;
		}
	}
	if(access(rel_path.c_str(),0)!=0)//access�����ǲ鿴�ļ��ǲ��Ǵ���
	{
		if (MKDIR(rel_path .c_str()))//��������ھ���mkdir����������
		{
			cout << "create folder[" << rel_path.c_str() <<"]" << " error" << endl;
			return 0;
		}
	}
	file_frame ins("1");
	ins.set_opt( file_frame::process, _proc);
	ins.set_opt( file_frame::filter, _filter);
	ins.set_opt( file_frame::statistic, _statistic);
	ins.set_opt( file_frame::showstatistic, _show);
	ins.set_opt( file_frame::data, NULL);
	ins.add_dir(src_path.c_str(), tar_path.c_str());
	ins.start();

	file_frame ins2("1");
	ins2.set_opt( file_frame::process, _proc2);
	ins2.set_opt( file_frame::filter, _filter2);
	ins2.set_opt( file_frame::statistic, _statistic);
	ins2.set_opt( file_frame::showstatistic, _show);
	ins2.set_opt( file_frame::data, NULL);
	ins2.add_dir(src_path.c_str(), rel_path.c_str());
	ins2.start();

	file_frame ins3("1");
	ins3.set_opt( file_frame::process, _proc3);
	ins3.set_opt( file_frame::filter, _filter);
	ins3.set_opt( file_frame::statistic, _statistic);
	ins3.set_opt( file_frame::showstatistic, _show);
	ins3.set_opt( file_frame::data, NULL);
	ins3.add_dir(tar_path.c_str(), rel_path.c_str());
	ins3.start();
	return 0;
}