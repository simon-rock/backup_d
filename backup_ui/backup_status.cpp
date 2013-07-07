#include "backup_status.h"

void backup_status::control_by_brick(ctr_type _t, string _brick_id, const char* msg)
{
    lock l(&m_lock);
    switch(_t)
    {
        case CTR_ADD:
            m_brick_status[_brick_id];
            m_brick_status[_brick_id]._stat = "STOP";
            break;
        case CTR_START:
            m_brick_status[_brick_id]._stat = "START";
            break;
        case CTR_STOP:
            m_brick_status[_brick_id]._stat = "STOP";
            break;
        case CTR_ERR:
            m_brick_status[_brick_id]._msg = msg;
            break;
        case CTR_MSG:
            m_brick_status[_brick_id]._msg = msg;
            break;
        default:
            break;
    }
}

string backup_status::print()
{
    lock l(&m_lock);
    string out;
    for(map<string, brick_stat>::iterator item = m_brick_status.begin(); item != m_brick_status.end(); ++item)
    {
        out += item->first;
        out += "\t";
        out += item->second._stat;
        out += "\t";
        out += item->second._msg;
        out += "\n";
    }
    return out;
}
