
#include "backup_main.h"
#include "brick_group.h"
#include <stdio.h> //For sprintf()
BackupMain::BackupMain()
    : m_Frame_Horizontal("SURDOC BACKUP"),
    m_Frame_Vertical("Vertical Button Boxes"),
    m_init(false),
    m_bn_stop_all("STOP")
{

    set_size_request(600, 400);
	int ret = pthread_create(&m_dismsg, 0, dispose_msg, (void*)this);
	if( ret == -1 )
	{
		cout << "create dispose thread error" << endl;
	}

    set_title("SURDOC BACKUP " VERSION);
    add(m_VBox_Main);
 //   m_VBox_Main.pack_start(m_Frame_Horizontal, Gtk::PACK_EXPAND_WIDGET, 10);
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS);
    m_VBox_Main.pack_start(m_ScrolledWindow, Gtk::PACK_EXPAND_WIDGET, 10);
//The horizontal ButtonBoxes:
    m_VBox.set_border_width(10);
//    m_Frame_Horizontal.add(m_VBox);
    m_ScrolledWindow.add(m_VBox);

// init backupd
	const char* db = "backup";
	const char* ip = "127.0.0.1";
	const char* user = "root";
	const char* psw = "root";
	if (backup_init(db, ip, user, psw) != 0)
	{
		cout << "init backupd error "<< endl;
	}

    while(!m_init)
    {
        usleep(1000000);    // 1 sec
    }

    m_bn_stop_all.signal_clicked().connect(sigc::mem_fun(*this, &BackupMain::on_button_clicked));
    show_all_children();
    // start backup
    backup_start();

}
BackupMain::~BackupMain()
{
}
void BackupMain::on_button_clicked()
{
    backup_stop();
    //hide();
}

int BackupMain::add_backup(string _brick_id, string _brick_backup_path)
{
    m_VBox.pack_start(*Gtk::manage(
                          new Brick_Ctrl(true, _brick_id.c_str(), _brick_backup_path.c_str(), 0,
                                  Gtk::BUTTONBOX_START)),
                      //Gtk::PACK_EXPAND_WIDGET, 5);
                      Gtk::PACK_SHRINK, 5);
    return 0;
}
void * BackupMain::dispose_msg(void* _pPara)
{
    BackupMain* pMain = static_cast<BackupMain*>(_pPara);

	while (1)
	{
		int imsg_type;
		int iRet = -1;
		int size = 0;

		void *para = NULL;
		iRet=backup_getmsg(imsg_type,para,size);

		while (iRet == -2)
		{
			para = realloc(para,sizeof(char)*size);
			iRet=backup_getmsg(imsg_type,para,size);
		}
		if (iRet == 0)
		{
			switch(imsg_type)
			{
			case MSG(START):
				cout << "backup service start " << endl;
				break;
			case MSG(STOP):
				cout << "backup service stop " << endl;
				break;
			case MSG(CONFIG):
				cout << "[BRICK_ID]" << ((MSG_DATA(CONFIG)*)para)->BRICK_ID
					 << "\n\t[BRICK_PATH]" << ((MSG_DATA(CONFIG)*)para)->BRICK_PATH
					 << "\n\t[MOUNT_PATH]" << ((MSG_DATA(CONFIG)*)para)->MOUNT_PATH
					 << "\n\t[BACKUP_PATH]" << ((MSG_DATA(CONFIG)*)para)->BACKUP_PATH
					 << endl;
                pMain->add_backup(((MSG_DATA(CONFIG)*)para)->BRICK_ID, ((MSG_DATA(CONFIG)*)para)->BACKUP_PATH);
				break;
            case MSG(CONFIG_COMPLETE):
				pMain->m_init = true;
				break;
			case MSG(BACKUP_BRICK):
				cout << "[BRICK_ID] " << ((MSG_DATA(BACKUP_BRICK)*)para)->BRICK_ID<< " start backup;" << endl;
				pMain->control_by_brick(CTR_START, ((MSG_DATA(BACKUP_BRICK)*)para)->BRICK_ID);
				break;
            case MSG(BACKUP_BRICK_STOP):
				cout << "[BRICK_ID] " << ((MSG_DATA(BACKUP_BRICK_STOP)*)para)->BRICK_ID<< " backup stop;" << endl;
				pMain->control_by_brick(CTR_STOP, ((MSG_DATA(BACKUP_BRICK_STOP)*)para)->BRICK_ID);
				break;
            case MSG(BACKUP_BRICK_ERROR):
				cout << "[BRICK_ID] " << ((MSG_DATA(BACKUP_BRICK_ERROR)*)para)->BRICK_ID<< " backup error;" << endl;
				pMain->control_by_brick(CTR_ERR, ((MSG_DATA(BACKUP_BRICK_ERROR)*)para)->BRICK_ID, ((MSG_DATA(BACKUP_BRICK_ERROR)*)para)->MSG_SHOW);
				break;
			case MSG(BACKUP_BRICK_MSG):
				cout << "[BRICK_ID] " << ((MSG_DATA(BACKUP_BRICK_MSG)*)para)->BRICK_ID<< " backup error;" << endl;
				pMain->control_by_brick(CTR_MSG, ((MSG_DATA(BACKUP_BRICK_MSG)*)para)->BRICK_ID, ((MSG_DATA(BACKUP_BRICK_ERROR)*)para)->MSG_SHOW);
				break;
			default:
				cout << "unknow msg" << endl;
				break;
			}
		}
		free(para);
	}
	return (void*)0;
}

void BackupMain::control_by_brick(ctr_type _t, string _brick_id, const char* msg)
{
    Glib::ListHandle< Widget* > widget_children = m_VBox.get_children();
    bool find = false;
    for(Glib::ListHandle< Widget* >::iterator item = widget_children.begin(); item != widget_children.end(); ++item)
    {
        if (static_cast<Brick_Ctrl*>(*item)->get_id() == _brick_id)
        {
            Brick_Ctrl* _tmp = static_cast<Brick_Ctrl*>(*item);
            switch(_t)
            {
                case CTR_START:
                _tmp->change_stat(Brick_Ctrl::BRI_START);
                    break;
                case CTR_STOP:
                _tmp->change_stat(Brick_Ctrl::BRI_STOP);
                    break;
                case CTR_ERR:
                _tmp->change_stat(Brick_Ctrl::BRI_ERR, msg);
                    break;
                case CTR_MSG:
                _tmp->change_stat(Brick_Ctrl::BRI_MSG, msg);
                    break;
                default:
                    break;
            }
            find = true;
        }
        if(find) break;
    }
    if (!find) cout << "can't find child_widget [" << _brick_id << "]" << endl;
}

bool BackupMain::on_window_state_event (GdkEventWindowState*event)
{
    fprintf(stderr, "Window State: %d\n", event->new_window_state);
    return true;
}
