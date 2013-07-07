#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

int main()
{
    ifstream in("/etc/mtab");
    if (!in)
    {
        cout << "open failed" << endl;
        return -1;
    }
    
    stringstream buf;
    buf << in.rdbuf();

    in .close();
    ofstream out("/etc/mtab", ios::out);
    if (!out)
    {
        cout << "write file error" << endl;
    }

    string line;
    while(getline(buf, line))
    {
        if (line.find("nfsd") == string::npos)
        {
            out << line << endl;
        }
    }
    return 0;
}
