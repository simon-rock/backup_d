#ifndef _BACKUP_WORKER_H_
#define _BACKUP_WORKER_H_
#include "com_inc.h"
#include "disk_container.h"
#include "file_frame.h"
#include "db_exec.h"
#include <vector>
using std::vector;
class backup_worker
{
public:
	backup_worker(string _brick_id, string _brick_path, string _src_path, string _tar_path, time_t _lastbackup_time, time_t _space_time, vector<unsigned int> &_pos)
		:m_brick_id(_brick_id),
		m_brick_path(_brick_path),
		m_src_path(_src_path),
		m_lastbackup_time(_lastbackup_time),
		m_space_time (_space_time),
		m_end_time(0),
		//m_stop(false),
		m_pause(false),
		m_frame(_brick_id)
		{

			if (_src_path.length() > 1 &&
				(_src_path[_src_path.length() - 1] == '\\' ||
				_src_path[_src_path.length() - 1] == '/'))
			{
				_src_path[_src_path.length() - 1] = '\0';
				//_src_path += DIS;
			}
			//m_src_path = _src_path;
			m_target.setdir(_tar_path, m_brick_id, _pos);

			count_file_err = 0;
			count_folder_err = 0;
			count_success = 0;
		}
	~backup_worker(){}
	int init();
	int start();
//	int restore();
private:
	backup_worker(backup_worker& _o);
	backup_worker operator =(backup_worker& _o);
	bool stop();	// whether stop backup this task
	int init_target();
	int init_src();
//	int pause();
	//
	static int _proc ( string SourcePath, string DestinationPath, string path, bool folder, void* pdata );
	static int _statistic ( int ,void* pdata);
	static int _show (void* pdata);
	static int _filter (struct _finddata_t*  pfileinfo, const char * file_path, void* pdata);
	//
	//bool m_stop;
	bool m_pause;			// no use
	string m_brick_id;		// brick_id
	string m_brick_path;	// brick net path
	string m_src_path;
	time_t m_lastbackup_time;
	time_t m_space_time;
	time_t m_end_time;

	// statistic
	long long count_file_err;
	long long count_folder_err;
	long long count_success;

	disk_container m_target;
	file_frame m_frame;
	db_exec m_db;
};

#endif //_BACKUP_WORKER_H_