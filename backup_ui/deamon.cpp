#include "deamon.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

#include "backup_main.h"
#include "backup_status.h"
#include <fcntl.h>
using namespace std;
static void * dispose_msg(void* _pPara);
static void * client_view(void* _pPara);
static void * client_ctr(void* _pPara);
bool _running = true;
void setdeamon()
{
    pid_t pc = fork();
	if(pc<0){
		printf("error fork\n");
		exit(1);
	}
	else if(pc>0)
		exit(0);
	setsid();
	chdir("/surlog");
	//umask(0);
	for(int i=0;i<MAXFILE;i++)
		close(i);
	signal(SIGTERM, sigterm_handler);
    // dispose msg
    pthread_t m_dismsg;
	int ret = pthread_create(&m_dismsg, 0, dispose_msg, NULL);
	if( ret == -1 )
	{
		cout << "create dispose thread error" << endl;
	}
	// client view
	pthread_t m_view;
	ret = pthread_create(&m_view, 0, client_view, NULL);
	if( ret == -1 )
	{
		cout << "create client_view thread error" << endl;
	}

	// client control
    pthread_t m_ctr;
    ret = pthread_create(&m_ctr, 0, client_ctr, NULL);
	if( ret == -1 )
	{
		cout << "create client_ctr thread error" << endl;
	}
}

void sigterm_handler(int arg)
{
	backup_stop();
	_running = false;
}

static void * dispose_msg(void* _pPara)
{
	while (_running)
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
                backup_status::instance()->control_by_brick(backup_status::CTR_ADD, ((MSG_DATA(BACKUP_BRICK)*)para)->BRICK_ID);
				break;
            case MSG(CONFIG_COMPLETE):
				break;
			case MSG(BACKUP_BRICK):
				backup_status::instance()->control_by_brick(backup_status::CTR_START, ((MSG_DATA(BACKUP_BRICK)*)para)->BRICK_ID);
				break;
            case MSG(BACKUP_BRICK_STOP):
                backup_status::instance()->control_by_brick(backup_status::CTR_STOP, ((MSG_DATA(BACKUP_BRICK_STOP)*)para)->BRICK_ID);
				break;
            case MSG(BACKUP_BRICK_ERROR):
                backup_status::instance()->control_by_brick(backup_status::CTR_ERR, ((MSG_DATA(BACKUP_BRICK_ERROR)*)para)->BRICK_ID, ((MSG_DATA(BACKUP_BRICK_ERROR)*)para)->MSG_SHOW);
				break;
			case MSG(BACKUP_BRICK_MSG):
                backup_status::instance()->control_by_brick(backup_status::CTR_MSG, ((MSG_DATA(BACKUP_BRICK_MSG)*)para)->BRICK_ID, ((MSG_DATA(BACKUP_BRICK_ERROR)*)para)->MSG_SHOW);
				break;
			default:
				cout << "unknow msg" << endl;
				break;
			}
		}
		free(para);

	}
	return (void*)0;
}

static void * client_view(void* _pPara)
{
    int fd;
    if((mkfifo(FIFOVIEW,O_CREAT|O_EXCL)<0)&&(errno!=EEXIST))
        printf("cannot create fifoserver\n");
    while (_running)
    {
        fd=open(FIFOVIEW, O_WRONLY, 0);
        string tmp = backup_status::instance()->print();
        write(fd,tmp.c_str(),tmp.size() + 1);
        sleep(2);
    }
    return (void*)0;
}

static void * client_ctr(void* _pPara)
{
    char r_buf[1024];
    int ret_size;
    while (_running)
    {
        int fd=open(FIFOCONTROL, O_RDONLY, 0);
     	if(fd==-1)
        {
            // printf("open %s for read error\n", FIFO_SERVER);
        }
        memset(r_buf,0,sizeof(r_buf));
        ret_size=read(fd,r_buf,1000);
        if(ret_size==-1)
        {
            // print error
            continue;
        }
        printf("%s\n", r_buf);
        string strctl = r_buf;
        if(strctl.size() > 2)
        {
            switch (strctl[0])
            {
                case '0':   //  START
//                printf("start : %s (%u)\n", strctl.substr(2).c_str(), strctl.substr(2).length());
                backup_startbybrick(strctl.substr(2).c_str());
                break;
                case '1':   // STOP
//                printf("stop : %s (%u)\n", strctl.substr(2).c_str(), strctl.substr(2).length());
                backup_stopbybrick(strctl.substr(2).c_str());
                case '9':   // EXIST
                backup_stop();
                _running = false;
                break;
                default:
                    break;
            }
        }
        close(fd);
    }
    return (void*)0;
}
