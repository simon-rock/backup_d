#include "backup_task.h"
#include "backup_worker.h"
#include "msg_queue.h"
void proc(task_data* data)
{
	backup_task* _data = static_cast<backup_task*>(data);

	LOG(INFO) << "[brick_id]" << _data->brick_id << "[mount_path]" << _data->mount_path << "[backup_path]" << _data->backup_path << "[last_filter_time]" << _data->last_filter_time << "[last_filter_interval]" << _data->last_filter_interval;
	FlushLogFiles(INFO);

	// reg error
	if (!task_control::Instance()->reg(_data->brick_id))
	{
		LOG(WARNING) << "[ERROR][brick_id]" << _data->brick_id << "[mount_path]" << _data->mount_path << "[backup_path]" << _data->backup_path << "[last_filter_time]" << _data->last_filter_time << "[last_filter_interval]" << _data->last_filter_interval;
		FlushLogFiles(WARNING);
		return;
	}
	backup_worker worker(_data->brick_id, _data->brick_path, _data->mount_path, _data->backup_path, _data->last_filter_time /*1344585483 - 60*60*24*30*4*//*1036800*/ /*1343548683*/, _data->last_filter_interval /*60*60*24*//*86400*/, _data->pos);

	//////////////////////////////////////////////////////////////////////////
	//MSG_DATA(BACKUP_BRICK) *pmsg_data_b= new MSG_DATA(BACKUP_BRICK);
	//memset(pmsg_data_b,0,sizeof(MSG_DATA(BACKUP_BRICK)));
	//strncpy(pmsg_data_b->BRICK_ID, _data->brick_id.c_str(), _data->brick_id.size() < sizeof(pmsg_data_b->BRICK_ID) ?
	//	_data->brick_id.size() : sizeof(pmsg_data_b->BRICK_ID) - 1);

	//MSG_DATA(BACKUP_BRICK_STOP) *pmsg_data_e= new MSG_DATA(BACKUP_BRICK_STOP);
	//memset(pmsg_data_e,0,sizeof(MSG_DATA(BACKUP_BRICK_STOP)));
	//strncpy(pmsg_data_e->BRICK_ID, _data->brick_id.c_str(), _data->brick_id.size() < sizeof(pmsg_data_e->BRICK_ID) ?
	//	_data->brick_id.size() : sizeof(pmsg_data_e->BRICK_ID) - 1);

	//MSG_DATA(BACKUP_BRICK_ERROR) *perr_msg_data= new MSG_DATA(BACKUP_BRICK_ERROR);
	//memset(perr_msg_data,0,sizeof(MSG_DATA(BACKUP_BRICK_ERROR)));
	//strncpy(perr_msg_data->BRICK_ID, _data->brick_id.c_str(), _data->brick_id.size() < sizeof(perr_msg_data->BRICK_ID) ?
	//	_data->brick_id.size() : sizeof(perr_msg_data->BRICK_ID) - 1);
	DEFINEMSGDATA(pmsg_data_b, BACKUP_BRICK);
	COPYDATA(pmsg_data_b->BRICK_ID, _data->brick_id.c_str(), _data->brick_id.size());

	DEFINEMSGDATA(pmsg_data_e, BACKUP_BRICK_STOP);
	COPYDATA(pmsg_data_e->BRICK_ID, _data->brick_id.c_str(), _data->brick_id.size());

	DEFINEMSGDATA(perr_msg_data, BACKUP_BRICK_ERROR);
	COPYDATA(perr_msg_data->BRICK_ID, _data->brick_id.c_str(), _data->brick_id.size());
	//////////////////////////////////////////////////////////////////////////
	switch(worker.init())
	{
	case BK_SUCESS:
		//POSTMESSAGE(MSG(BACKUP_BRICK),pmsg_data_b,sizeof(MSG_DATA(BACKUP_BRICK)));
		POSTMESSAGE(BACKUP_BRICK,pmsg_data_b);
		switch(worker.start())
		{
		case BK_STOP:
			LOG(INFO) << "[brick_id]" << _data->brick_id << "[result]" << "BK_STOP" ;
				break;
		case BK_SUCESS:
			LOG(INFO) << "[brick_id]" << _data->brick_id << "[result]" << "BK_SUCESS" ;
				break;
		default:
			LOG(INFO) << "[brick_id]" << _data->brick_id << "[result]" << "UNKNOW ERROR" ;
			strcpy(perr_msg_data->MSG_SHOW, "UNKNOW ERROR");
			break;
		}
		//POSTMESSAGE(MSG(BACKUP_BRICK_STOP),pmsg_data_e,sizeof(MSG_DATA(BACKUP_BRICK_STOP)));
		POSTMESSAGE(BACKUP_BRICK_STOP,pmsg_data_e);
		break;
	case BK_INIT_ERR:		// 失败
		LOG(WARNING) << "[brick_id]" << _data->brick_id << "[result]" << "BK_INIT_ERR" ;
		strcpy(perr_msg_data->MSG_SHOW, "BK_INIT_ERR");
		break;
	case BK_INIT_NEED_DEL:	// 插入盘不是空 已用空间超过50%
		LOG(WARNING) << "[brick_id]" << _data->brick_id << "[result]" << "BK_INIT_NEED_DEL";
		strcpy(perr_msg_data->MSG_SHOW, "BK_INIT_NEED_DEL");
		break;
	case BK_INIT_NO_DISK_MOUNT:	// 没有硬盘挂在到备份目录
		LOG(WARNING) << "[brick_id]" << _data->brick_id << "[result]" << "BK_INIT_NO_DISK_MOUNT";
		strcpy(perr_msg_data->MSG_SHOW, "BK_INIT_NO_DISK_MOUNT");
		break;
	case BK_INIT_ADD_DISK:	// 没有硬盘挂在到备份目录
		LOG(WARNING) << "[brick_id]" << _data->brick_id << "[result]" << "BK_INIT_ADD_DISK";
		strcpy(perr_msg_data->MSG_SHOW, "BK_INIT_ADD_DISK");
		break;
	default:				// 未知错误
		LOG(WARNING) << "[brick_id]" << _data->brick_id << "[result]" << "INIT UNKNOW ERROR";
		strcpy(perr_msg_data->MSG_SHOW, "INIT UNKNOW ERROR");
		break;
	}
	FlushLogFiles(INFO);
	FlushLogFiles(WARNING);
	task_control::Instance()->unreg(_data->brick_id);
	if (strlen(perr_msg_data->MSG_SHOW) != 0)
	{
		//POSTMESSAGE(MSG(BACKUP_BRICK_ERROR),perr_msg_data,sizeof(MSG_DATA(BACKUP_BRICK_ERROR)));
		POSTMESSAGE(BACKUP_BRICK_ERROR,perr_msg_data);
	}
}

bool task_control::reg(const string& _brick_id)
{
	lock l(&m_lock);
	for(list<task_sem*>::iterator item = _v.begin(); item != _v.end(); ++item) 
	{ 
		if(!(*item)->brick_id.compare(_brick_id)) 
		{ 
			LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [unreg] have registed to control";
			FlushLogFiles(INFO);
			return false;
		} 
	}
	task_sem *_s = new task_sem();
	_s->brick_id = _brick_id;
#ifdef SYS_WINDOWS
	_s->sem = CreateEventA(NULL, TRUE, FALSE,"");
#endif //SYS_WINDOWS
#ifdef SYS_LINUX
	sem_init(&(_s->sem), 0, 0);
#endif // SYS_LINUX
	_v.push_back(_s);
	return true;
}
void task_control::unreg(const string& _brick_id)
{
	//iterator new_end = remove_if(_v.begin(), _v.end(),);
	//array.erase(new_end);
	lock l(&m_lock);
	for(list<task_sem*>::iterator item = _v.begin(); item != _v.end(); ++item) 
	{ 
		if(!(*item)->brick_id.compare(_brick_id)) 
		{ 
#ifdef SYS_WINDOWS
			CloseHandle ((*item)->sem);
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
			sem_destroy(&((*item)->sem));
#endif // SYS_LINUX
			item = _v.erase(item);
			return;
		} 
	}
	LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [unreg] have't registed to control";
	FlushLogFiles(INFO);
}
void task_control::set_sem(const string& _brick_id)
{
	lock l(&m_lock);
	for(list<task_sem*>::iterator item = _v.begin(); item != _v.end(); ++item) 
	{ 
		if(!(*item)->brick_id.compare(_brick_id)) 
		{ 
#ifdef SYS_WINDOWS
			SetEvent((*item)->sem);
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
			//if (sem_trywait(&m_htask) == EAGAIN)
			//{
			int ret = sem_post(&((*item)->sem));
			if ( 0 != ret )
			{
				LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [set_sem] [error]";
				FlushLogFiles(INFO);
				//		cout<<"cannot signal event"<<endl;
			}
			//}
#endif //SYS_LINUX
			LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [set_sem]";
			FlushLogFiles(INFO);
			return;
		} 
	}
	LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [setsem] have't registed to control";
	FlushLogFiles(INFO);
}

// warining : if infinite == true; it happen deadlock, because m_lock will not be release.set_all ,set_sem not hold the lock !!!
bool task_control::wait_sem(const string& _brick_id, bool infinite)
{
	lock l(&m_lock);
	for(list<task_sem*>::iterator item = _v.begin(); item != _v.end(); ++item) 
	{ 
		if(!(*item)->brick_id.compare(_brick_id)) 
		{ 
#ifdef SYS_WINDOWS
			int ret = WaitForSingleObject((*item)->sem, infinite?INFINITE:0);
			if (ret == WAIT_OBJECT_0)
			{
				return true;
			}
			else if(ret == WAIT_TIMEOUT)
			{
				return false;
			}
			LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [waitsem] error";
			FlushLogFiles(INFO);
			return false;
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
			if (infinite)
			{
				int ret = sem_wait(&((*item)->sem));
				if ( 0 != ret )
				{
					LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [wait_sem][error]";
					FlushLogFiles(INFO);
					return false;
					//		cout<<"CEventImpl::WaitImpl sem_wait failed"<<endl; 
				}
				sem_post(&((*item)->sem));
				return true;
			}
			else
			{
				int ret = sem_trywait(&((*item)->sem));
				if ( 0 != ret )
				{
					// switch (ret)
						// if no signed, return false;
					switch (errno)
					{
					case EINTR:
						LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [wait_sem error][EINTR:" << EINTR<< "]" << ret;
						break;
					case EAGAIN:
					//	LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [wait_sem error][EAGAIN:" << EAGAIN << "]" << ret;		// no signal
						break;
					case EINVAL:
						LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [wait_sem error][EINVAL:" << EINVAL << "]" << ret;
						break;
					case ETIMEDOUT:
						LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [wait_sem error][ETIMEDOUT" << ETIMEDOUT << "]" << ret;
						break;
					default:
						LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [wait_sem error][UNKNOW]" << ret;
					}
					FlushLogFiles(INFO);
					
					return false;
				}
				sem_post(&((*item)->sem));
				return true;
			}

#endif // SYS_LINUX
		} 
	}
	LOG(WARNING) << "[BTICK_ID]" << _brick_id  << ": [waitsem]have't registed to control";
	FlushLogFiles(INFO);
	return false;
}

void task_control::set_all()
{
	lock l(&m_lock);
	LOG(INFO) << "stop all task";
	FlushLogFiles(INFO);
	for(list<task_sem*>::iterator item = _v.begin(); item != _v.end(); ++item) 
	{ 
#ifdef SYS_WINDOWS
			SetEvent((*item)->sem);
#endif // SYS_WINDOWS
#ifdef SYS_LINUX
			int ret = sem_post(&((*item)->sem));
			if ( 0 != ret )
			{
				LOG(WARNING) << "[BTICK_ID]" << (*item)->brick_id  << ": [set_sem] [error]";
				FlushLogFiles(INFO);
				//		cout<<"cannot signal event"<<endl;
			}
#endif // SYS_LINUX
	}
}