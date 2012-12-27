// backup.h
#ifndef _BACKUP_H_
#define _BACKUP_H_

// -1 : error [please check log]
int backup_init(const char* db, const char* ip, const char* user, const char* psw);

// stop backup service and start all task
int backup_start();

// stop spec. brick backup 
void backup_stopbybrick(const char *brick_id);

// start spec. brick backup 
void backup_startbybrick(const char *brick_id);

//  stop backup, prepare to stop ap.
int backup_stop();

// ��ȡ����ʱ��Ϣ
// msg_num			: ��Ϣ����
// para				: ��Ϣ����
// para_length		: ���������Ϣ�����Ŀռ䳤�ȣ�������Ϣ�������ȣ��ռ䲻��ʱ����-2
// ����ֵ			: 0 �ɹ��� -1 ��ȡ��Ϣʧ�ܻ�-2 ����ռ䲻��
int backup_getmsg(int &msg_num, void *para, int &para_length);

#endif // _BACKUP_H_