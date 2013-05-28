#ifndef  _DB_EXEC_H_
#define  _DB_EXEC_H_
#include "com_inc.h"
#include "mysqlplus.h"
#include <sstream>
using std::stringstream;
class config;
class db_exec
{
public:
	db_exec();
	~db_exec(){}
	int update_backup_time(const string& _brick_id, long long _begin_time);
	int add_disk_for_brick(const string& _brick_id, const string& _disk_id);
	int add_file_log(const string& _disk_id, const string& _file);
	// have backuped :1; no backuped :0; error :-1; 
	int isbackuped(const string& _disk_id, const string& _file);

private:
	static void init(const string& _db_name,const string& _ip,const string& _user,const string& _psw);
	// exist: 1; noexist: 0; error: BK_DB_ERROR
	int isExistdisk(const string& _brick_id, const string& _disk_id);
	int exec(const string& _sql);
	db_exec(const db_exec&);
	db_exec operator = (const db_exec&);
	
	static string m_db_name;
	static string m_ip;
	static string m_user;
	static string m_psw;

	sql_connection_c m_connection;
	sql_query_c m_query;
	stringstream  m_sql;
// friend  class
	friend class config;
};


#endif //_CONFIG_H_