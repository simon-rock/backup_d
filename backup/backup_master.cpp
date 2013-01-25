#include "backup_master.h"
#include "thread_pool.h"
#include "backup_task.h"
#include "msg_queue.h"
int backup_master::init(const char *db_name, const char* ip, const char * user, const char* psw)
{
	// glog
	InitGoogleLogging("backup");
	SetLogDestination(INFO,"./log/info_");
	SetLogDestination(WARNING,"./log/error_");
	SetLogDestination(FATAL,"");

    // check user id
    if (getuid() != 0) {
		fprintf(stderr, "erro root privileges needed \n");
        LOG(WARNING) << "erro root privileges needed ";
		FlushLogFiles(INFO);
        return BK_ERROR;
	}
    
	// init pref
	if (m_pref.login_db(db_name, ip, user, psw) == BK_SUCESS &&
		m_pref.check() == BK_SUCESS)
	{
		//POSTMESSAGE(MSG(CONFIG_COMPLETE),NULL,0);
		POSTMESSAGENODATA(CONFIG_COMPLETE);
		return BK_SUCESS;
	}
	//POSTMESSAGE(MSG(CONFIG_COMPLETE),NULL,0);
	POSTMESSAGENODATA(CONFIG_COMPLETE);
	return BK_ERROR;
}

int backup_master::start()
{
	// 根据配置信息启动任务

	thread_pool* pool = thread_pool::getInstance();
	pool->initialize(m_pref.m_task.size(), m_pref.m_task.size());
	for (vector<backup_task*>::iterator item = m_pref.m_task.begin(); item != m_pref.m_task.end(); ++item)
	{
		backup_task* data = new backup_task(**item);
		pool->add_task(proc, data);
	}
	// 开始任务
	pool->process();
	//usleep(1000*60*3);
	//task_control::Instance()->set_sem(string("NAS-1-BRICK-1"));
	//pool->shut_down();
	//POSTMESSAGE(MSG(START),NULL,0);
	POSTMESSAGENODATA(START);
	return BK_SUCESS;
}

int backup_master::start_by_brick(string _brick_id)
{
	int succ = BK_ERROR;
	for (vector<backup_task*>::iterator item = m_pref.m_task.begin(); item != m_pref.m_task.end(); ++item)
	{
		if ((*item)->brick_id == _brick_id)
		{
			backup_task* data = new backup_task(**item);
			thread_pool::getInstance()->add_task(proc, data);
			thread_pool::getInstance()->process();
			succ = BK_SUCESS;
		}
		if (succ == BK_SUCESS)
		{
			break;
		}
	}
	return succ;
}

int backup_master::stop()
{
	task_control::Instance()->set_all();
	thread_pool::getInstance()->shut_down();
	//POSTMESSAGE(MSG(STOP),NULL,0);
	POSTMESSAGENODATA(STOP);
	return BK_SUCESS;
}
int backup_master::uninit()
{
	for (vector<backup_task*>::iterator item = m_pref.m_task.begin(); item != m_pref.m_task.end(); ++item)
	{
		delete(*item);
	}
	return BK_SUCESS;
}
