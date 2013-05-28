#include "db_exec.h"

string db_exec::m_db_name = "";
string db_exec::m_ip	= "";
string db_exec::m_user	= "";
string db_exec::m_psw	= "";
void db_exec::init(const string& _db_name,const string& _ip,const string& _user,const string& _psw)
{
	m_db_name	=  _db_name;
	m_ip		=  _ip;
	m_user		=  _user;
	m_psw		=  _psw;
}

db_exec::db_exec():
	m_connection(m_db_name.c_str(), m_ip.c_str(), m_user.c_str(), m_psw.c_str()),
	m_query( &m_connection )
{
}

int db_exec::update_backup_time(const string& _brick_id, long long _begin_time)
{
	m_sql << "update BRICK_MASTER set BACKUP_TIME_BEGIN = " << _begin_time << " where BRICK_ID = \"" <<  _brick_id << "\";";
	return exec(m_sql.str());
}
int db_exec::add_file_log(const string& _disk_id, const string& _file)
{
	m_sql << "insert into DISK_FILE values(\"" << _disk_id << "\",\"" <<  _file << "\");";
	return exec(m_sql.str());
}
int db_exec::isbackuped(const string& _disk_id, const string& _file)
{
	m_sql << "select BK_FILE from DISK_FILE where BK_FILE = \"" << _file << "\";" ;
	if ( !m_query.execute( m_sql.str().c_str() ))
	{
		return BK_DB_ERROR;
	}
	m_sql.str("");
	sql_result_c *sql_result = m_query.store();
	if(sql_result->n_rows() > 0)
	{
		return BK_DB_EXIST;
	}
	else
	{
		return BK_DB_NOEXIST;
	}
}

int db_exec::isExistdisk(const string& _brick_id, const string& _disk_id)
{

	m_sql << "select DISK_ID from BRICK_DISK where BRICK_ID = \"" << _brick_id << "\" and DISK_ID = \"" << _disk_id << "\";" ;
	if ( !m_query.execute( m_sql.str().c_str() ))
	{
		return BK_DB_ERROR;
	}
	m_sql.str("");
	sql_result_c *sql_result = m_query.store();
	if(sql_result->n_rows() > 0)
	{
		return BK_DB_EXIST;
	}
	else
	{
		return BK_DB_NOEXIST;
	}
}
int db_exec::add_disk_for_brick(const string& _brick_id, const string& _disk_id)
{
	if (isExistdisk(_brick_id, _disk_id) == BK_DB_EXIST)
	{
		return BK_DB_SUCESS;
	}
	
	m_sql << "insert into BRICK_DISK values(\"" << _brick_id << "\",\"" << _disk_id << "\");";
	return exec(m_sql.str());
}

int db_exec::exec(const string& _sql)
{
	int ret = BK_DB_SUCESS;
	if ( m_query.exec_sql( _sql.c_str() ) == -1)
	{
		ret = BK_DB_ERROR;
	}
	m_sql.str("");
	return ret;
}