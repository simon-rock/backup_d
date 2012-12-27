#include <string>
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#define BUF_MAX 1024
#include <iostream>
#include <tr1/memory>
#include <map>
static char* pbuf = NULL;

using namespace std;
using std::tr1::shared_ptr;
typedef struct _ctrl_info
{
    string _sas_address;
    int _num;
}ctrl_info;

void list_sas_address()
{
    FILE *stream;
    pbuf = (char *)malloc(sizeof(char)* BUF_MAX);
    memset(pbuf, '\0', sizeof(char)*BUF_MAX);
    stream = popen("./lsiutil.x86_64 -s", "r");

    string sbuf;
    // ctrl
    map<string, shared_ptr<ctrl_info> > ctrl_pool;
    // disk
    string sas_address;
    int num = 0;
    while(fgets(pbuf, 1024, stream) != NULL)
    {
        sbuf = pbuf;
        if (sbuf.find("EnclServ") != string::npos)
        {
            shared_ptr<ctrl_info> o(new ctrl_info);
            string::size_type index = sbuf.rfind(' ');
            if (index != string::npos)
            {
                //string tmp = sbuf.substr(index);
                //num = atoi(tmp.c_str());
                o->_num = atoi(sbuf.substr(index).c_str());
                sbuf = sbuf.substr(0, index);
                if((index = sbuf.find_last_not_of(' ')) != string::npos)
                {
                    sbuf = sbuf.substr(0, index + 1);
                    if ((index = sbuf.rfind(' ')) != string::npos)
                    {
                        o->_sas_address = sbuf.substr(index + 1);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }

            if (o->_sas_address.size() > 12)
            {
                ctrl_pool[o->_sas_address.substr(0, 12)] = o;
            }
            //cout << sas_address << ":" << num << endl;
            //sas_address = "";
            //num = 0;
        }
        else if (sbuf.find("Disk") != string::npos)
        {
            string::size_type index = sbuf.rfind(' ');
            if (index != string::npos)
            {
                num = atoi(sbuf.substr(index).c_str());
                sbuf = sbuf.substr(0, index);
                if((index = sbuf.find_last_not_of(' ')) != string::npos)
                {
                    sbuf = sbuf.substr(0, index + 1);
                    if ((index = sbuf.rfind(' ')) != string::npos)
                    {
                        sas_address = sbuf.substr(index + 1);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
            cout << sas_address << ":" << num << endl;
            sas_address = "";
            num = 0;
        }
    }

    for(map<string, shared_ptr<ctrl_info> >::iterator item = ctrl_pool.begin();
    item != ctrl_pool.end(); ++item)
    {
        cout << item->first << " : " << item->second->_sas_address << " : " << item->second->_num << endl;   
    }
    pclose(stream);
}

string query_disk(string const& p)
{
    FILE *stream;
    pbuf = (char *)malloc(sizeof(char)* BUF_MAX);
    memset(pbuf, '\0', sizeof(char)*BUF_MAX);
    string strcmd;
    strcmd =  "udevadm info -a -p $(udevadm info -q path -n";
    strcmd += p;
    strcmd += ") | grep ATTRS{sas_address}";
    stream = popen(strcmd.c_str(), "r");

    string sbuf;
    // ctrl
    map<string, shared_ptr<ctrl_info> > ctrl_pool;
    // disk
    string sas_address;
    int num = 0;
    while(fgets(pbuf, 1024, stream) != NULL)
    {
        sbuf = pbuf;
        string::size_type index;
        if ((index = sbuf.find("\"0x")) != string::npos)
        {
            cout << sbuf.substr(index + 3, sbuf.size() - index -3) << endl;
            pclose(stream);
            return sbuf.substr(index + 3, sbuf.size() - index - 5);
        }
        cout << sbuf << endl;
    }
    pclose(stream);
    return "";
}
int main()
{
    // list_sas_address();
    string ret = query_disk("/dev/sdc");
    cout << "ret = " << ret << endl;
    return 0;
}
