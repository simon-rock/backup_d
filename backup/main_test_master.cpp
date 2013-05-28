#include "backup_master.h"

int main(int argc, char **argv)
{
	backup_master ins;
	const char* db = "backup";
	const char* ip = "127.0.0.1";
	const char* user = "root";
	const char* psw = "root";
	if (ins.init(db, ip, user, psw) == BK_ERROR)
	{
//		cout << "error" << endl;
		return -1;
	}
	ins.start();
	//task_control::Instance()->set_sem(); 停止单独任务
	usleep(1000000*60*2);
	ins.stop();								// 停止
	//while(1)
	//{
	//	Sleep(1000);

	//}
}
