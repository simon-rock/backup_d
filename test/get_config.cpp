#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    string ex = " cong = wer # sdfsf";
    string::size_type index_be, index_end;
    index_be = ex.find_first_of("#");
    index_end = ex.find_first_not_of(" ");
    
    string dbname = "backup";
    string host   = "127.0.0.1";
    string user   = "root";
    string psw    = "root";
    
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
        cout << config_name << "| : |" << config << "|"<< endl;
        string buff = config_name;
        config_name.assign(buff.begin() + buff.find_first_not_of(' '),
                           buff.begin() + buff.find_last_not_of(' ') + 1);
        buff = config;
        config.assign(buff.begin() + buff.find_first_not_of(' '),
                           buff.begin() + buff.find_last_not_of(' ') + 1);
        cout << config_name << "| ... |" << config << "|"<< endl;

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

    cout << host << ":" << dbname << ":" << user << ":" << psw << endl;
    return 0;
}
