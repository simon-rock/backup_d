#ifndef _BACKUP_MAIN_H_
#define _BACKUP_MAIN_H_
#include <gtkmm.h>
//
#include "com_inc.h"
#include "backup.h"
#include "msg_queue.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
//
using std::cout;
using std::endl;
class BackupMain : public Gtk::Window
{
public:
    enum ctr_type
    {
        CTR_START = 0,
        CTR_STOP,
        CTR_MSG,
        CTR_ERR
    };

    BackupMain();
    virtual ~BackupMain();
protected:
//Signal handlers:
    virtual void on_button_clicked();
    virtual bool on_window_state_event (GdkEventWindowState*event);

//Child widgets:
    Gtk::VBox m_VBox_Main, m_VBox;
    Gtk::HBox m_HBox;
    Gtk::Frame m_Frame_Horizontal, m_Frame_Vertical;    // replace by m_ScrolledWindow
    Gtk::ScrolledWindow m_ScrolledWindow;

private:
    void control_by_brick(ctr_type _t, string _brick_id, const char* msg = NULL);
    static void * dispose_msg(void* _pPara);
    int add_backup(string _brick_id, string _brick_backup_path);// add backup info group
    pthread_t m_dismsg;                                        // dispose msg thread
    bool m_init;                                                // whether init complete
    Gtk::Button m_bn_stop_all;                                  // stop all task (not used)
};
#endif //_BACKUP_MAIN_H_
