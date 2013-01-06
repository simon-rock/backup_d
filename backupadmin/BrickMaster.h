#ifndef _BRICKMASTER_H_
#define _BRICKMASTER_H_
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <string>
#include <vector>
#include "db_config.h"
using std::string;
using std::vector;
class MasterWin;
class BrickMaster : public Gtk::Frame
{
public:
    BrickMaster(const MasterWin* _pp);
    virtual ~BrickMaster();
    void on_flush();
    void on_flush_pos();
    void on_add();
    void on_del();
    void on_add_disk_pos();     // add disk pos for sp. brick
    void on_del_disk_pos();     // delete disk pos for sp. brick
    void on_check_date();
    void on_select_mount_path();
	void on_select_backup_path();
private:
    Gtk::VBox m_main;
    Gtk::Button m_add, m_del, m_check_date, m_flush, m_browse_mount_path, m_browse_backup_path, m_add_disk_pos, m_del_disk_pos;

    Gtk::OptionMenu m_OptionNASMenu, m_OptionIndexMenu, m_OptionBrickMenu, m_OptionDiskPosForBrickMenu;
    Gtk::Entry m_EntryBrickId, m_EntryDiskPos, m_EntryPath, m_EntryMountPath, m_EntryBackPath, m_EntryBeginDate, m_EntryInterval;
    Gtk::Label m_LPos, m_LBrickId, m_LPath, m_LMountPath, m_LBackPath, m_LBeginDate, m_LInterval, m_date;
    Gtk::Menu m_NAS_menu, m_Index_menu, m_Brick_menu, m_Disk_pos_menu;
    vector<string> m_curr_nas_v, m_curr_index_v, m_curr_brick_v, m_curr_pos_v;

MasterWin* pp;
db_config m_db;
};
#endif // _BRICKMASTER_H_
