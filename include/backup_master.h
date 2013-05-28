#ifndef _BACKUP_MASTER_H_
#define  _BACKUP_MASTER_H_
#include "com_inc.h"
#include "sys_config.h"
class backup_master
{
public:
	backup_master(void){};
	~backup_master(void){};
	int init(const char *db_name, const char* ip, const char * user, const char* psw);		// init all config for backup
	int start();		// start backup
	int stop();			// 
	int start_by_brick(string _brick_id);
	int uninit();	
private:
	config m_pref;
};

#endif //_BACKUP_MASTER_H_
