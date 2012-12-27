#include "db_config.h"

string db_config::m_db_name = "";
string db_config::m_ip	= "";
string db_config::m_user	= "";
string db_config::m_psw	= "";
int db_config::init(const string& _db_name,const string& _ip,const string& _user,const string& _psw)
{
	sql_connection_c connection( _db_name.c_str(), _ip.c_str(), _user.c_str(), _psw.c_str());
	sql_query_c query( &connection );
	if (query.ping())
	{
		return BK_DB_ERROR;
	}

	m_db_name	=  _db_name;
	m_ip		=  _ip;
	m_user		=  _user;
	m_psw		=  _psw;
	return BK_DB_SUCESS;
}

db_config::db_config():
	m_connection(m_db_name.c_str(), m_ip.c_str(), m_user.c_str(), m_psw.c_str()),
	m_query( &m_connection )
{
}

int db_config::add_nas(const string& _nas)
{
    m_sql << "insert into NAS_MASTER values(\"" << _nas << "\");";
    return exec(m_sql.str());
}
int db_config::del_nas(const string& _nas)
{
    m_sql << "delete from NAS_MASTER where NAS_ID = \"" << _nas << "\";";
    return exec(m_sql.str());
}
int db_config::add_index(const string& _index)
{
    m_sql << "insert into BRICK_INDEX values(" << _index << ");";
	return exec(m_sql.str());
}
int db_config::del_index(const string& _index)
{
    m_sql << "delete from BRICK_INDEX where BRICK_NUM = " << _index << ";";
    return exec(m_sql.str());
}
int db_config::add_brick(const string& _nas,
                         const string& _index,
                         const string& _brick_id,
                         const string& _path,
                         const string& _mountpath,
                         const string& _backuppath,
                         const string& _begintime,
                         const string& _interval)
{
    m_sql << "insert into BRICK_MASTER values(\"" << _brick_id << "\",\"" << _path << "\",\"" << _mountpath << "\",\"" << _backuppath << "\"," << _begintime << "," << _interval << ");";
    if (exec(m_sql.str()) == BK_DB_ERROR) return BK_DB_ERROR;

    m_sql << "insert into BACKUP_MASTER values(\"" << _nas << "\"," << _index << ",\"" << _brick_id << "\");";
    if(exec(m_sql.str()) == BK_DB_ERROR)
    {
        m_sql << "delete from BRICK_MASTER where BRICK_ID = \"" << _brick_id << "\";";
        if (exec(m_sql.str()) == BK_DB_ERROR)
        {
            // ***************************
            // trans process yuyu
            return BK_DB_ERROR;
        }
        else
        {
            return BK_DB_ERROR;
        }
    }
    return BK_DB_SUCESS;
}
int db_config::del_brick(const string& _brick_id)
{
    m_sql << "delete from BACKUP_MASTER where BRICK_ID = \"" << _brick_id << "\";";
    if (exec(m_sql.str()) == BK_DB_SUCESS)
    {
        m_sql << "delete from BRICK_MASTER where BRICK_ID = \"" << _brick_id << "\";";

        // ***************************
        // trans process yuyu
        return exec(m_sql.str());
    }
    else
    {
        return BK_DB_ERROR;
    }

}
int db_config::exec(const string& _sql)
{
	int ret = BK_DB_SUCESS;
	if ( m_query.exec_sql( _sql.c_str() ) == -1)
	{
		ret = BK_DB_ERROR;
	}
	m_sql.str("");
	return ret;
}

int db_config::query_nas(vector<string> &_v)
{
    m_sql << "select NAS_ID from NAS_MASTER;";
	if ( m_query.execute( m_sql.str().c_str() ))
	{
        sql_result_c *sql_result = m_query.store();
        long long n_rows = sql_result->n_rows();
        int n_fields = sql_result->n_fields();
        for (int r_idx = 0; r_idx < n_rows; r_idx++)
        {
            sql_row_c sql_row = sql_result->fetch_row();
            sql_row.allow_null(0);
            for(int j = 0; j < n_fields; ++j)
            {
                _v.push_back((char*)sql_row[j]);
            }
        }
	}
	else
	{
	    // no data(0) or query error(-1)
	}

    m_sql.str("");
    return BK_DB_SUCESS;
}
int db_config::query_index(vector<string> &_v)
{
    m_sql << "select BRICK_NUM from BRICK_INDEX;";
	if ( m_query.execute( m_sql.str().c_str() ))
	{
        sql_result_c *sql_result = m_query.store();
        long long n_rows = sql_result->n_rows();
        int n_fields = sql_result->n_fields();
        for (int r_idx = 0; r_idx < n_rows; r_idx++)
        {
            sql_row_c sql_row = sql_result->fetch_row();
            sql_row.allow_null(0);
            for(int j = 0; j < n_fields; ++j)
            {
                _v.push_back((char*)sql_row[j]);
            }
        }
	}
	else
	{
	    // no data(0) or query error(-1)
	}

    m_sql.str("");
    return BK_DB_SUCESS;
}
int db_config::query_brick_id(vector<string> &_v)
{
    m_sql << "select BRICK_ID from BRICK_MASTER;";
	if ( m_query.execute( m_sql.str().c_str() ))
	{
        sql_result_c *sql_result = m_query.store();
        long long n_rows = sql_result->n_rows();
        int n_fields = sql_result->n_fields();
        for (int r_idx = 0; r_idx < n_rows; r_idx++)
        {
            sql_row_c sql_row = sql_result->fetch_row();
            sql_row.allow_null(0);
            for(int j = 0; j < n_fields; ++j)
            {
                _v.push_back((char*)sql_row[j]);
            }
        }
	}
	else
	{
	    // no data(0) or query error(-1)
	}

    m_sql.str("");
    return BK_DB_SUCESS;
}

int db_config::check_nas_index(const string& _nas, const string& _index)
{
	m_sql << "select * from BACKUP_MASTER where NAS_ID = \"" << _nas << "\" and BRICK_NUM = \"" << _index << "\";" ;
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
