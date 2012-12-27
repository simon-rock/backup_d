#ifndef _BACKUP_STATUS_H_
#define _BACKUP_STATUS_H_
#include <map>
#include <string>
#include "lock.h"
using std::string;
using std::map;
class backup_status
{
public:
    enum ctr_type
    {
        CTR_ADD = 0,
        CTR_START,
        CTR_STOP,
        CTR_MSG,
        CTR_ERR
    };
	static backup_status* instance()
	{
		static backup_status ins;
		return &ins;
	}
    void control_by_brick(ctr_type _t, string _brick_id, const char* msg = NULL);
    string print();
    ~backup_status(){}
private:
    class brick_stat
    {
        public:
        string _stat;
        string _msg;
    };

    backup_status(){}
    void operator = (backup_status& _o);
    backup_status(backup_status& _o);
    map<string, brick_stat> m_brick_status;
    lock_obj m_lock;
};
#endif // _BACKUP_STATUS_H_
