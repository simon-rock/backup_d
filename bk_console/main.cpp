#include "com_inc.h"
#include "backup.h"
#include "msg_queue.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
using std::cout;
using std::endl;
/home/gaoyu/workspace/mysql/include
#ifdef SYS_WINDOWS
unsigned int __stdcall dispose_msg(void* _pPara)
#endif// SYS_WINDOWS
#ifdef SYS_LINUX
void * dispose_msg(void* _pPara)
#endif// SYS_LINUX
{
	while (1)
	{
		int imsg_type;
		int iRet = -1;
		int size = 0;

		void *para = NULL;
		iRet=backup_getmsg(imsg_type,para,size);

		while (iRet == -2)
		{
			para = realloc(para,sizeof(char)*size);
			iRet=backup_getmsg(imsg_type,para,size);
		}
		if (iRet == 0)
		{
			switch(imsg_type)
			{
			case MSG(START):
				cout << "backup service start " << endl;
				break;
			case MSG(STOP):
				cout << "backup service stop " << endl;
				break;
			case MSG(CONFIG):
				cout << "[BRICK_ID]" << ((MSG_DATA(CONFIG)*)para)->BRICK_ID 
					 << "\n\t[BRICK_PATH]" << ((MSG_DATA(CONFIG)*)para)->BRICK_PATH 
					 << "\n\t[MOUNT_PATH]" << ((MSG_DATA(CONFIG)*)para)->MOUNT_PATH 
					 << "\n\t[BACKUP_PATH]" << ((MSG_DATA(CONFIG)*)para)->BACKUP_PATH 
					 << endl; 
				break;
			case MSG(BACKUP_BRICK):
				cout << "[BRICK_ID] " << ((MSG_DATA(BACKUP_BRICK)*)para)->BRICK_ID<< " start backup;" << endl;
				break;
			default:
				cout << "unknow msg" << endl;
				break;
			}
		}
		free(para);
	}
#ifdef SYS_WINDOWS
	return 0;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	return (void*)0;
#endif // SYS_LINUX
}

int main()
{
	// init msg
	unsigned int m_uithreadID;

#ifdef SYS_WINDOWS
	HANDLE m_hthread = (HANDLE)_beginthreadex( NULL,
		0,
		&dispose_msg,
		NULL,
		0,
		&m_uithreadID );
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
	pthread_t	m_hthread;
	int ret = pthread_create(&m_hthread, 0, dispose_msg, NULL);
	if( ret == -1 )
	{
		// error
	}
#endif // SYS_LINUX
	const char* db = "backup";
	const char* ip = "127.0.0.1";
	const char* user = "root";
	const char* psw = "root";
	if (backup_init(db, ip, user, psw) != 0)
	{
		cout << "init  error "<< endl;
	}
	else
	{
		backup_start();
		//task_control::Instance()->set_sem(); 停止单独任务
		usleep(1000000*60*2);
		backup_stop();							// 停止
	}
	
#ifdef SYS_WINDOWS
	system("pause");
#endif //SYS_WINDOWS
#ifdef SYS_LINUX
	getchar();
#endif // SYS_LINUX
	return 0;
}