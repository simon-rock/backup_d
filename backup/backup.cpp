#include "backup.h"
#include "backup_master.h"
#include "backup_task.h"
#include "msg_queue.h"
#include <cstdlib>
backup_master ins;
int backup_init(const char* db, const char* ip, const char* user, const char* psw)
{
	if (ins.init(db, ip, user, psw) == BK_ERROR)
	{
		//		cout << "error" << endl;
		return -1;
	}
	else
	{
		return 0;
	}
}

int backup_start()
{
	return ins.start();
}

void backup_stopbybrick(const char* brick_id)
{
	task_control::Instance()->set_sem(string(brick_id));
}

// start spec. brick backup 
void backup_startbybrick(const char *brick_id)
{
	ins.start_by_brick(brick_id);
}

int backup_stop()
{
	return ins.stop();
}

// ��ȡsurdoc ����ʱ��Ϣ
// msg_num			: ��Ϣ����
// para				: ��Ϣ����
// para_length		: ���������Ϣ�����Ŀռ䳤�ȣ�������Ϣ�������ȣ��ռ䲻��ʱ����-2
// ����ֵ			: 0 �ɹ��� -1 ��ȡ��Ϣʧ�ܻ�-2 ����ռ䲻��
int backup_getmsg(int &msg_num, void *para, int &para_length)
{
	//if (para == NULL)
	//{
	//	return -1;
	//}
	if (msg_queue::instance()->wait_msg())
	{
		msg *tmp = msg_queue::instance()->front();
		msg_num = tmp->msg_num;
		if (para_length < tmp->para_length )
		{
			para_length = tmp->para_length;
			return -2;
		}
		else
		{
			para_length = tmp->para_length;
		}
		msg_queue::instance()->pop_front();
		if (tmp->para != NULL)
		{
			memcpy(para, tmp->para, tmp->para_length);
			// delete tmp->para; // don't delete void *
			free (tmp->para);
		}
		delete tmp;
		return 0;
	}
	return -1;
}