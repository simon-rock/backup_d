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

// 获取运行时消息
// msg_num			: 消息类型
// para				: 消息参数
// para_length		: 输入接受消息参数的空间长度，返回消息参数长度，空间不足时返回-2
// 返回值			: 0 成功； -1 获取消息失败或；-2 分配空间不足
int backup_getmsg(int &msg_num, void *para, int &para_length);

#endif // _BACKUP_H_