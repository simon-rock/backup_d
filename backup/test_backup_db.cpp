#include "db_exec.h"
#include <iostream>
using namespace std;
int main()
{
	const char* db = "backup";
	const char* ip = "192.168.1.156";
	const char* user = "root";
	const char* psw = "root";

	db_exec::init(db, ip, user, psw);

	string brick_id = "NAS-1-BRICK-1";
	long long begin_time = 2222222;
	string file = "filename";
	string disk_id = "disk_1";
	db_exec _t;
	if (_t.update_backup_time(brick_id, begin_time) == BK_DB_ERROR)
	{
		cout << "update_backup_time error " << endl; 
	}
	if (_t.add_disk_for_brick(brick_id, disk_id) == BK_DB_ERROR)
	{
		cout << "add_disk_for_brick error " << endl;
	}
	if (_t.add_file_log(disk_id, file) == BK_DB_ERROR)
	{
		cout << "add_file_log error " << endl;
	}
	if (_t.isbackuped(disk_id, file) == BK_DB_ERROR)
	{
		cout << "isbackuped error" << endl;
	}
	


	return 0;
}