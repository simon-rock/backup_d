
#include "backup_main.h"
#include "brick_group.h"
#include <stdio.h> //For sprintf()
ExampleWindow::ExampleWindow()
    : m_Frame_Horizontal("Horizontal Button Boxes"),
    m_Frame_Vertical("Vertical Button Boxes"),
    m_init(false)
{

	int ret = pthread_create(&m_hthread, 0, dispose_msg, (void*)this);
	if( ret == -1 )
	{
		// error
	}

    set_title("Gtk::ButtonBox");
    add(m_VBox_Main);
    m_VBox_Main.pack_start(m_Frame_Horizontal, Gtk::PACK_EXPAND_WIDGET, 10);
//The horizontal ButtonBoxes:
    m_VBox.set_border_width(10);
    m_Frame_Horizontal.add(m_VBox);

    while(!m_init)
    {
        usleep(1000000);    // 1 sec
    }
 /*   m_VBox.pack_start(*Gtk::manage(
                          new ExampleButtonBox(true, "Start (spacing 20)", 20,
                                  Gtk::BUTTONBOX_START)),
                      Gtk::PACK_EXPAND_WIDGET, 5);
*/

    show_all_children();
}
ExampleWindow::~ExampleWindow()
{
}
void ExampleWindow::on_button_clicked()
{
    hide();
}

int ExampleWindow::add_backup(string _brick_id, string _brick_backup_path)
{
    m_VBox.pack_start(*Gtk::manage(
                          new ExampleButtonBox(true, _brick_id.c_str(), 20,
                                  Gtk::BUTTONBOX_START)),
                      Gtk::PACK_EXPAND_WIDGET, 5);
}
void * ExampleWindow::dispose_msg(void* _pPara)
{
    const ExampleWindow* pMain = static_cast<const ExampleWindow*>(_pPara);

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
			case MSG(BACKUP_BRICK):
				cout << "[BRICK_ID] " << ((MSG_DATA(BACKUP_BRICK)*)para)->BRICK_ID<< " start backup;" << endl;
				break;
            case MSG(CONFIG_COMPLETE):
				pMain->m_init = true;
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
