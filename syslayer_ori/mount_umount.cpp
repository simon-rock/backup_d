#include <fstream>
#include <sstream>
#include <sys/mount.h>
#include <iostream>
#include <string.h>
using namespace  std;

bool mount_local_ext4(string const& _dev, string const & _mount_path)
{
    if (mount(_dev.c_str(),
              _mount_path.c_str(),
              "ext4",
              MS_MGC_VAL|MS_RDONLY, "") == 0)
    {
        ofstream o("/etc/mtab", ios::app);
        if (o)
        {
            o << _dev << " " << _mount_path << " ext4 rw 0 0" << endl;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool umount_local_path(string const& _path)
{
    fstream file("/etc/mtab", ios::in| ios::out);
    if (!file)
    {
        return false;
    }
    
    if (umount(_path.c_str())== 0)
    {
     
        stringstream buf;
        buf << file.rdbuf();

        file.close();
        ofstream out("/etc/mtab", ios::out);
        if (!out)
        {
            cout << "write file error" << endl;
            return true;
        }

        string line;
        while(getline(buf, line))
        {
            if (line.find(_path + " ") == string::npos)
            {
                out << line << endl;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cout << "need parm " << endl;
        return -1;
    }
    if (strcmp(argv[1], "1")== 0)
    {
        if (mount_local_ext4(argv[2], argv[3]))
        {
            cout << "mount ["
                 << argv[2] << "] to ["
                 << argv[3] << "] sucess"
                 << endl;
        }
        else
        {
            cout << "mount failed" << endl;
        }
        
    }
    else if (strcmp(argv[1], "2") == 0)
    {
        if (umount_local_path(argv[2]))
        {
            cout << "umount ["
                 << argv[2] << "] sucess"
                 << endl;
        }
        else
        {
            cout << "umount failed" << endl;
        }
    }
    return 0;
}
