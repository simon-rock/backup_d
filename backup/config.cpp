#include "sys_config.h"
#include "msg_queue.h"
#include "mysqlplus.h"
#include "db_exec.h"
#include "sysinfod.h"
int config::login_db(const char *db_name, const char* ip, const char * user, const char* psw)
{
	if (db_name == NULL||
		ip == NULL ||
		user == NULL||
		psw == NULL)
	{
		return BK_ERROR;
	}
	sql_connection_c connection( db_name, ip, user, psw);
	sql_query_c query( &connection );
	sql_result_c *sql_result = 0;
	if (query.ping())
	{
		LOG(WARNING) << "can't connect to mysql";
		return BK_ERROR;
	}
	
	// no data(0) or query error(-1)
	if ( !query.execute( "select BRICK_ID, PATH, MOUNT_PATH, BACKUP_PATH, BACKUP_TIME_BEGIN, BACKUP_TIME_INTERVAL from BRICK_MASTER;" ) )
	{
		LOG(WARNING) << "can't get BRICK_MASTER data";
		return BK_ERROR;
	}
	//
	m_db_name = db_name;
	m_ip = ip;
	m_user = user;
	m_psw = psw;

	db_exec::init(m_db_name, m_ip, m_user, m_psw);
	//
	sql_result = query.store();
	long long n_rows = sql_result->n_rows();
	
	for (int r_idx = 0; r_idx < n_rows; r_idx++)
	{
		sql_row_c sql_row = sql_result->fetch_row();
		sql_row.allow_null(0);

		backup_task* p = new backup_task();
		p->brick_id = (char*)sql_row[0];
		p->brick_path = (char*)sql_row[1];
		p->mount_path = (char*)sql_row[2];
		p->backup_path = (char*)sql_row[3];
		p->last_filter_time = (int)sql_row[4];
		p->last_filter_interval = (int)sql_row[5];

        // format path ,delete '/'
        if (!(p->mount_path.empty()))
        {
            if (p->mount_path[p->mount_path.size() - 1] == '/')
            {
                p->mount_path = p->mount_path.substr(0, p->mount_path.size() - 1);
            }
        }
        if (!(p->backup_path.empty()))
        {
            if (p->backup_path[p->backup_path.size() - 1] == '/')
            {
                p->backup_path = p->backup_path.substr(0, p->backup_path.size() - 1);
            }
        }
            //     
		if (!p->brick_id.empty())
		{
			string sql = "select DISK_POS from BRICK_DISK_POS where BRICK_ID = \"" + p->brick_id + "\";";
			// no data(0) or query error(-1)
			if ( !query.execute( sql.c_str() ))
			{
				continue;
			}
			sql_result_c *_result = 0;
			_result = query.store();
			long long n_pos = _result->n_rows();
			for (int i = 0; i < n_pos; i++)
			{
				sql_row_c pos = _result->fetch_row();
				p->pos.push_back((int)pos[0]);
			}
		}
		// POSTMESSAGE
		#ifdef SYS_WINDOWS
		//MSG_DATA(CONFIG) * pmsg_data= new MSG_DATA(CONFIG);
		//memset(pmsg_data,0,sizeof(MSG_DATA(CONFIG)));
		//////int t = sizeof(pmsg_data->BRICK_ID);
		//////strncpy(pmsg_data->BRICK_ID, p->brick_id.c_str(), p->brick_id.size());
		//strncpy(pmsg_data->BRICK_ID, p->brick_id.c_str(), p->brick_id.size() < sizeof(pmsg_data->BRICK_ID) ?
		//	p->brick_id.size() : sizeof(pmsg_data->BRICK_ID) - 1);
		//strncpy(pmsg_data->BRICK_PATH, p->brick_path.c_str(), p->brick_path.size() < sizeof(pmsg_data->BRICK_PATH) ? 
		//	p->brick_path.size() : sizeof(pmsg_data->BRICK_PATH) - 1);
		//strncpy(pmsg_data->MOUNT_PATH, p->mount_path.c_str(), p->mount_path.size() < sizeof(pmsg_data->MOUNT_PATH) ? 
		//	p->mount_path.size() : sizeof(pmsg_data->MOUNT_PATH) - 1);
		//strncpy(pmsg_data->BACKUP_PATH, p->backup_path.c_str(), p->backup_path.size() < sizeof(pmsg_data->BACKUP_PATH) ? 
		//	p->backup_path.size() : sizeof(pmsg_data->BACKUP_PATH) - 1);
		//POSTMESSAGE(MSG(CONFIG),pmsg_data,sizeof(MSG_DATA(CONFIG)));
		#endif // SYS_WINDOWS
		DEFINEMSGDATA(pmsg_data, CONFIG);
		COPYDATA(pmsg_data->BRICK_ID, p->brick_id.c_str(), p->brick_id.size());
		COPYDATA(pmsg_data->BRICK_PATH, p->brick_path.c_str(), p->brick_path.size());
		COPYDATA(pmsg_data->MOUNT_PATH, p->mount_path.c_str(), p->mount_path.size());
		COPYDATA(pmsg_data->BACKUP_PATH, p->backup_path.c_str(), p->backup_path.size());
		POSTMESSAGE(CONFIG,pmsg_data);
		// POSTMESSAGE
		m_task.push_back(p);
	}

    // init syslayer info
    vector<sysinfod::jbod_info> vj;
    sysinfod::jbod_info ji;
    sql_query_c query_jbod_info( &connection );
    if(!query_jbod_info.execute("select *from JBOD_MASTER"))
    {
        LOG(WARNING) << " can't get JBOD_MASTER data";
        return BK_ERROR;
    }
    sql_result = query_jbod_info.store();
    n_rows = sql_result->n_rows();

    for (int r_idx = 0; r_idx < n_rows; r_idx++)
    {
        sql_row_c sql_row = sql_result->fetch_row();
        sql_row.allow_null(0);
        
        ji._sas_address = (char*) sql_row[0];
        ji._start_num = (int)sql_row[1];
        ji.asc_or_des = ((int)sql_row[2] == 1)? true:false;
        vj.push_back(ji);
    }
    sysinfod::getInstance()->set_jbod_info(vj);
    sysinfod::getInstance()->init_disk_info();
	return BK_SUCESS;
}

int config::check()
{
	// ¼ìÑéÊý¾Ý
	return BK_SUCESS;
}
