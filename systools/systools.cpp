#include "systools.h"
#include "com_inc.h"

#ifdef SYS_LINUX
#include <sys/vfs.h>    /* or <sys/statfs.h> */
#endif // SYS_LINUX
#include <fstream>
using std::ifstream;
void itoa ( unsigned long val, char *buf, unsigned radix )
{
	char   *p;							
	char   *firstdig;                   
	char   temp;                        
	unsigned   digval;					

	p = buf;   
	firstdig = p;                       

	do{   
		digval = (unsigned)(val%radix);   
		val /= radix;					

		if   (digval > 9)   
		*p++ = (char)(digval - 10 + 'a');     
		else   
		*p++   =   (char)(digval + '0');      
	}while (val > 0);   

	*p-- = '\0';

	do{   
		temp = *p;   
		*p = *firstdig;   
		*firstdig = temp;
		--p;   
		++firstdig;
	}while (firstdig < p);
}

int getdiskspaceinfo(const char *dir, unsigned long long& freebycall, unsigned long long& total, unsigned long long& free)
{
	if (dir == NULL)
	{
		return -1;
	}
#ifdef SYS_WINDOWS
	if (GetDiskFreeSpaceExA(dir, (PULARGE_INTEGER)&freebycall, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free))
	{
		return 0;
	}
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	struct statfs fs;
	if(statfs(dir, &fs) == 0)
	{
		total = fs.f_blocks*fs.f_bsize;
		freebycall = fs.f_bavail*fs.f_bsize;
		free = fs.f_bfree*fs.f_bsize;
		return 0;
	}

#endif //  SYS_LINUX
	return -1;
}

long long getfilesize(const char *dir)
{
#ifdef SYS_WINDOWS
	struct _stat64 buf;
	//errno_t err;
	return _stat64( dir, &buf ) == 0?buf.st_size:-1;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	struct stat64 buf; 
	return stat64(dir, &buf) == 0?buf.st_size:-1;
#endif // SYS_LINUX
}

long long getfile_mtime(const char *dir)
{
#ifdef SYS_WINDOWS
	struct _stat64 buf;
//	errno_t err;
	return _stat64( dir, &buf ) == 0?buf.st_mtime:-1;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	struct stat64 buf; 
	return stat64(dir, &buf) == 0?buf.st_mtime:-1;
#endif // SYS_LINUX
}

int isfolder(const char *dir)
{
#ifdef SYS_WINDOWS
	struct _stat64 buf;
	return _stat64( dir, &buf ) == 0?(((buf.st_mode&_S_IFDIR) == _S_IFDIR)?1:0):0;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	struct stat64 buf; 	
	return stat64(dir, &buf) == 0?(((buf.st_mode&S_IFMT) == S_IFDIR)?1:0):0;
#endif // SYS_LINUX
}

int get_dbconfig(string& db_name, string& host, string& user, string& psw)
{
    db_name = "backup";
    host = "127.0.0.1";
    user = "root";
    psw  = "root";
    
    ifstream in("backup_config");
    string line, content, config, config_name;
    string::size_type begin_index, end_index, config_begin;
    while (!in.eof()
        &&! in.fail())
    {
        config = "";
        config_name = "";
        getline(in, line);
        end_index = line.find_first_of("#");
        begin_index = line.find_first_not_of(" ");
        if (begin_index == string::npos)
        {
            continue;
        }
        if (end_index == string::npos)
        {
            end_index = line.size();
        }
        if (begin_index <  end_index)
        {
            content = line.substr(begin_index, end_index - begin_index);
            if ((config_begin = content.find("=")) != string::npos) 
            {
                config = content.substr(config_begin + 1);
                config_name = content.substr(0, config_begin);
            }
            else
                continue;
        }
        else
            continue;

        if (config_name.empty() ||
            config.empty())
        {
            continue;
        }

        // format string, del space
        string buff = config_name;
        config_name.assign(buff.begin() + buff.find_first_not_of(' '),
                           buff.begin() + buff.find_last_not_of(' ') + 1);
        buff = config;
        config.assign(buff.begin() + buff.find_first_not_of(' '),
                           buff.begin() + buff.find_last_not_of(' ') + 1);

        if (!config_name.compare("USER"))
        {
            user = config;   
        }
        else if(!config_name.compare("PSW"))
        {
            psw = config;
        }
        else if(!config_name.compare("HOST"))
        {
            host = config;
        }
    }

    return 0;
}
