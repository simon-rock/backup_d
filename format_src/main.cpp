﻿#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "file_frame.h"
#include "systools.h"
//////////////////////src 处理--del SYS_WINDOWS////////////////////////////////////////////////////
int _proc(string SourcePath, string DestinationPath, string path, bool folder, void* pdata)
{
	string src_path = (SourcePath + path).c_str();
	string tar_path = (DestinationPath + path).c_str();
    cout << src_path << endl;
	if (folder)
	{
		MKDIR(tar_path.c_str());
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
#ifdef SYS_WINDOWS
	if (pfileinfo != NULL)
	{
		if ((pfileinfo->attrib & _A_SUBDIR) == _A_SUBDIR)
		{
			if (string(pfileinfo->name) != "Pre-built.2" &&
				string(pfileinfo->name) != "Debug" &&
				string(pfileinfo->name) != "Release" &&
				string(pfileinfo->name) != "glog-0.3.2" &&
				string(pfileinfo->name) != "glog_project" &&
				string(pfileinfo->name) != "test" &&
				string(pfileinfo->name) != "sql_wrapper" &&
				string(pfileinfo->name) != "format_src" &&
				string(pfileinfo->name) != "glog" &&
				string(pfileinfo->name) != ".git" &&
				string(pfileinfo->name) != "backup_ui_w" &&
				string(pfileinfo->name) != "bk_console" &&
				string(pfileinfo->name) != "lib" &&
				string(pfileinfo->name) != "log" &&
				string(pfileinfo->name) != "obj" &&
                string(pfileinfo->name) != "syslayer_ori" &&
				string(pfileinfo->name) != "bin")
			{
				return FRAME_FILTER_IN;
			}else
			{
				return FRAME_FILTER_E;
			}
			
		}
		
		char   ext[50]; 
		_splitpath(pfileinfo->name, NULL, NULL, NULL, ext);
		if (string(pfileinfo->name) != "del_line.cpp" &&			// syslayer/
			string(pfileinfo->name) != "mount_umount.cpp" &&        // syslayer/ mount umount disk
			string(pfileinfo->name) != "sas_address.cpp" &&         // syslayer/ 
			string(pfileinfo->name) != "test.cpp" &&                // test syslayer
			string(pfileinfo->name) != "main_msg.cpp" &&			// backup/ test msg
			string(pfileinfo->name) != "main_test_master.cpp" &&    // backup/ test backup_master
			string(pfileinfo->name) != "main_thread.cpp" &&         // backup/ test thread pool
			string(pfileinfo->name) != "test_backup_db.cpp" &&      // backup/ test db_exe
			string(pfileinfo->name) != "test_main_backup.cpp" &&    // backup/ test backup system
			string(pfileinfo->name) != "test_task_control.cpp" &&   // backup/ test control system
			(string(ext) == ".cpp" ||
			string(ext) == ".h") )
		{
			return FRAME_FILTER_IN;
		}
	}
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
    if (file_path != NULL)
    {
        string path = file_path;
        string filename, ext;
        string::size_type index;
        if ((index = path.rfind('/')) != string::npos)
        {
            filename = path.substr(index + 1);
            if ((index = filename.rfind('.')) != string::npos)
            {
                ext = filename.substr(index);
            }
        }
        
        if (isfolder(file_path))
		{
			if (filename != "Pre-built.2" &&
				filename != "Debug" &&
				filename != "Release" &&
				filename != "glog-0.3.2" &&
                filename != "glog_project" &&
				filename != "test" &&
				filename != "sql_wrapper" &&
				filename != "format_src" &&
				filename != "glog" &&
				filename != ".git" &&
				filename != "backup_ui_w" &&
				filename != "bk_console" &&
				filename != "lib" &&
				filename != "log" &&
				filename != "obj" &&
				filename != "bin" &&
                filename != "syslayer_ori" &&
                filename != ".deps")
			{
				return FRAME_FILTER_IN;
			}else
			{
				return FRAME_FILTER_E;
			}
		}
		if (filename != "del_line.cpp" &&			// syslayer/
			filename != "mount_umount.cpp" &&        // syslayer/ mount umount disk
			filename != "sas_address.cpp" &&         // syslayer/ 
			filename != "test.cpp" &&                // test syslayer
			filename != "main_msg.cpp" &&			// backup/ test msg
			filename != "main_test_master.cpp" &&    // backup/ test backup_master
			filename != "main_thread.cpp" &&         // backup/ test thread pool
			filename != "test_backup_db.cpp" &&      // backup/ test db_exe
			filename != "test_main_backup.cpp" &&    // backup/ test backup system
			filename != "test_task_control.cpp" &&   // backup/ test control system
			(ext == ".cpp" ||
			ext == ".h") )
		{
			return FRAME_FILTER_IN;
		}
    }
#endif // SYS_LINUX
	return FRAME_FILTER_E;
}

//////////////////////删除makefile和其他工程文件和测试文件 处理--copy////////////////////////////////////////////////////
int _proc2(string SourcePath, string DestinationPath, string path, bool folder, void* pdata)
{
	string src_path = (SourcePath + path).c_str();
	string tar_path = (DestinationPath + path).c_str();
	if (folder)
	{
		MKDIR(tar_path.c_str());
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
#ifdef SYS_WINDOWS
	if (pfileinfo != NULL)
	{
		if ((pfileinfo->attrib & _A_SUBDIR) == _A_SUBDIR)
		{
			if (string(pfileinfo->name) != "Pre-built.2" &&
				string(pfileinfo->name) != "Debug" &&
				string(pfileinfo->name) != "Release" &&
				string(pfileinfo->name) != "glog-0.3.2" &&
				string(pfileinfo->name) != "glog_project" &&
				string(pfileinfo->name) != "test" &&
				string(pfileinfo->name) != "sql_wrapper" &&
				string(pfileinfo->name) != "format_src" &&
				string(pfileinfo->name) != "glog" &&
				string(pfileinfo->name) != ".git" &&
				string(pfileinfo->name) != "backup_ui_w" &&
				string(pfileinfo->name) != "bk_console" &&
				string(pfileinfo->name) != "lib" &&
				string(pfileinfo->name) != "log" &&
				string(pfileinfo->name) != "obj" &&
				string(pfileinfo->name) != "syslayer_ori" && // 原来的syslayer 可以处理jbod 信息处理硬盘位置，现在不使用
				string(pfileinfo->name) != "bin" )
			{
				return FRAME_FILTER_IN;
			}else
			{
				return FRAME_FILTER_E;
			}
		}
		

		char   ext[50]; 
		_splitpath(pfileinfo->name, NULL, NULL, NULL, ext);
		//if (string(pfileinfo->name) == "Makefile" ||
		//	string(ext) == ".sql")
		if (string(pfileinfo->name) != "Makefile_s" &&
			string(pfileinfo->name) != "Makefile_d" &&
			string(pfileinfo->name) != "Makefile_test" &&
			string(pfileinfo->name) != "Makefile_bak" &&
			string(pfileinfo->name) != ".gitignore" &&
			string(pfileinfo->name) != ".gitignore" &&
			string(pfileinfo->name) != "cmd" &&
			string(pfileinfo->name) != "Makefile" &&
			string(pfileinfo->name) != "README.md" &&
            string(pfileinfo->name) != "missing_ori" &&
            string(pfileinfo->name) != "depcomp_ori" &&
            string(pfileinfo->name) != "install-sh_ori" &&
			string(ext) != ".cpp" &&
			string(ext) != ".h" &&
			string(ext) != ".suo" &&
			string(ext) != ".sln" &&
			string(ext) != ".ncb" &&
			string(ext) != ".user" &&
			string(ext) != ".vcproj"&&
			string(ext) != ".txt" &&
			string(ext) != ".c" &&
			string(ext) != ".layout" &&
			string(ext) != ".cbp" &&
			string(ext) != ".o" &&
			string(ext) != ".depend"
			)
		{
			return FRAME_FILTER_IN;
		}
	}
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
    if (file_path != NULL)
    {
        string path = file_path;
        string filename, ext;
        string::size_type index;
        if ((index = path.rfind('/')) != string::npos)
        {
            filename = path.substr(index + 1);
            if ((index = filename.rfind('.')) != string::npos)
            {
                ext = filename.substr(index);
            }
        }
        
        if (isfolder(file_path))
        {
            if (filename != "Pre-built.2" &&
				filename != "Debug" &&
				filename != "Release" &&
				filename != "glog-0.3.2" &&
				filename != "glog_project" &&
				filename != "test" &&
				filename != "sql_wrapper" &&
				filename != "format_src" &&
				filename != "glog" &&
				filename != ".git" &&
				filename != "backup_ui_w" &&
				filename != "bk_console" &&
				filename != "lib" &&
				filename != "log" &&
				filename != "obj" &&
				filename != "bin" &&
				filename != "syslayer_ori" &&	// 原来的syslayer 可以处理jbod 信息处理硬盘位置，现在不使用
                filename != ".deps")
			{
				return FRAME_FILTER_IN;
			}else
			{
				return FRAME_FILTER_E;
			}
        }

        if (filename != "Makefile_s" &&
			filename != "Makefile_d" &&
			filename != "Makefile_test" &&
			filename != "Makefile_bak" &&
		    filename != ".gitignore" &&
			filename != ".gitignore" &&
			filename != "cmd" &&
			filename != "Makefile" &&
			filename != "test_syslayer" &&
			filename != "README.md" &&
            filename != "missing_ori" &&
            filename != "depcomp_ori" &&
            filename != "install-sh_ori" &&
			ext != ".cpp" &&
			ext != ".h" &&
			ext != ".suo" &&
			ext != ".sln" &&
			ext != ".ncb" &&
			ext != ".user" &&
			ext != ".vcproj"&&
			ext != ".txt" &&
			ext != ".c" &&
			ext != ".layout" &&
			ext != ".cbp" &&
			ext != ".o" &&
			ext != ".depend" &&
            ext != ".a"
			)
		{
			return FRAME_FILTER_IN;
		}
    }
#endif // SYS_LINUX
	return FRAME_FILTER_E;

}
//////////////////////src 处理--del SYS_LINUX////////////////////////////////////////////////////
int _proc3(string SourcePath, string DestinationPath, string path, bool folder, void* pdata)
{
	string src_path = (SourcePath + path).c_str();
	string tar_path = (DestinationPath + path).c_str();
	if (folder)
	{
		MKDIR(tar_path.c_str());
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
	if(access(tar_path.c_str(),0)!=0)//access函数是查看文件是不是存在
	{
		if (MKDIR(tar_path .c_str()))//如果不存在就用mkdir函数来创建
		{
			cout << "create folder[" << tar_path.c_str() <<"]" << " error" << endl;
			return 0;
		}
	}
	if(access(rel_path.c_str(),0)!=0)//access函数是查看文件是不是存在
	{
		if (MKDIR(rel_path .c_str()))//如果不存在就用mkdir函数来创建
		{
			cout << "create folder[" << rel_path.c_str() <<"]" << " error" << endl;
			return 0;
		}
	}
    cout << "-----first-----" << endl;
	// 删除源码中关于windows 的处理通过判断宏 SYS_WINDOWS
	file_frame ins("1");
	ins.set_opt( file_frame::process, _proc);
	ins.set_opt( file_frame::filter, _filter);
	ins.set_opt( file_frame::statistic, _statistic);
	ins.set_opt( file_frame::showstatistic, _show);
	ins.set_opt( file_frame::data, NULL);
	ins.add_dir(src_path.c_str(), tar_path.c_str());
	ins.start();
    cout << "-----second-----" << endl;
	// 拷贝需要文件， 删除makefile， 相关工程文件，测试文件，无用目录bin，debug，release等
	file_frame ins2("1");
	ins2.set_opt( file_frame::process, _proc2);
	ins2.set_opt( file_frame::filter, _filter2);
	ins2.set_opt( file_frame::statistic, _statistic);
	ins2.set_opt( file_frame::showstatistic, _show);
	ins2.set_opt( file_frame::data, NULL);
	ins2.add_dir(src_path.c_str(), rel_path.c_str());
	ins2.start();
    cout << "-----third-----" << endl;
	// 删除源码中 宏SYS_LINUX
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
