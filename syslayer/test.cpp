#include "sysinfod.h"
#include <map>
using std::map;
int main()
{
    vector<sysinfod::jbod_info> vj;
    sysinfod::jbod_info ji;
    ji._sas_address = "5003048001704e7d";
    ji._start_num = 12;
    ji.asc_or_des = true;
    vj.push_back(ji);
    sysinfod::getInstance()->set_jbod_info(vj);
    sysinfod::getInstance()->init_disk_info();
	sysinfod::getInstance()->print();
    cout << "test get_disk" << sysinfod::getInstance()->get_diskid("/home/yu/mount_test", 1) << endl;
/*
	char buf[1024];

	// disk id
	FILE *pf = fopen("disk_id.txt", "r");
	string disk;
	string id;
	map<string, string> mdisk_ip;
	bool bfind_disk = false;
	while(fgets(buf, 1024,pf) != NULL)
	{
		if (string(buf).find("Disk /dev/sd") != string::npos ||
			string(buf).find("Disk /dev/hd") != string::npos)
		{
			string::size_type index = string(buf).find(":");
			disk = string(buf).substr(strlen("Disk "), index - strlen("Disk "));
			bfind_disk = true;
		}

		if (bfind_disk)
		{
			if (string(buf).find("Disk identifier:") != string::npos)
			{
				id = string(buf).substr(strlen("Disk identifier: "), strlen("0x00000000"));
				mdisk_ip[disk] = id;
				bfind_disk = false;
			}
		}
	}

//////////////////////////////////////////////////////////////////////////
	// Partition_path.txt
	FILE *pf2 = fopen("Partition_path.txt", "r");
	string partition;
	string mountpath;
	map<string, string> mpartition_path;
	string::size_type index, space_index;

	while(fgets(buf, 1024,pf2) != NULL)
	{
		if ((index = string(buf).find(" on ")) != string::npos)
		{
			partition = string(buf).substr(0, index);

			mountpath = string(buf).substr(index + strlen(" on "));

			if ((space_index = mountpath.find(" "))!= string::npos)
			{
				mountpath = mountpath.substr(0, space_index);
				mpartition_path[mountpath] = partition;
			}
			else
			{
				mountpath = "";
			}
		}
	}
	*/
	return 0;
}
