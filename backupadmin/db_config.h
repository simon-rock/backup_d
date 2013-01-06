#ifndef _DB_EXEC_H_
#define _DB_EXEC_H_

#include "com_inc.h"
#include "mysqlplus.h"
#include <sstream>
#include <string>
#include <vector>
using std::stringstream;
using std::string;
using std::vector;
class db_config
{
public:
	db_config();
	~db_config(){}

    static int init(const string& _db_name,const string& _ip,const string& _user,const string& _psw);

    int add_nas(const string& _nas);
    int del_nas(const string& _nas);
    int add_index(const string& _index);
    int del_index(const string& _index);
    int add_brick(const string& _nas,
                  const string& _index,
                  const string& _brick_id,
                  const string& _path,
                  const string& _mountpath,
                  const string& _backuppath,
                  const string& _begintime,
                  const string& _interval);
    int del_brick(const string& _brick_id);
    int query_nas(vector<string> &_v);
    int query_index(vector<string> &_v);
    int query_brick_id(vector<string> &_v);
    int check_nas_index(const string& _nas, const string& _index);
    int add_disk_pos_to_brick(string const& _brick_id, string const& _pos);
    int del_disk_pos_to_brick(string const& _brick_id, string const& _pos);
    int query_disk_pos_by_brick(string const& _brick_id, vector<string> &_v);
private:

	int exec(const string& _sql);
	db_config(const db_config&);
	db_config operator = (const db_config&);

	static string m_db_name;
	static string m_ip;
	static string m_user;
	static string m_psw;

	sql_connection_c m_connection;
	sql_query_c m_query;
	stringstream  m_sql;
};

#endif // _DB_EXEC_H_
