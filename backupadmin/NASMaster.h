#ifndef _NASMASTER_H_
#define _NASMASTER_H_
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <string>
#include <vector>
#include "db_config.h"
using std::string;
using std::vector;
class MasterWin;
class NASMaster : public Gtk::Frame
{
public:
    NASMaster(const MasterWin* _pp);
    virtual ~NASMaster();
    void on_flush();
    void on_add_nas();
    void on_del_nas();
    void on_add_index();
    void on_del_index();
    //
    /*
static  const Gtk::Menu& Get_NASItems(){return m_NAS_menu;}
static  const Gtk::Menu& Get_IndexItems(){return m_Index_menu;}
static  const string Get_NAS_name(unsigned int index)
    {
        if(index < m_curr_nas_v.size()) return m_curr_nas_v[index];
        else
            return "";
    }
static  const string Get_index_name(unsigned int index)
    {
        if(index < m_curr_index_v.size()) return m_curr_index_v[index];
        else
            return "";
    }
    */
private:
    Gtk::VBox m_main, m_nas, m_brick_index;
    Gtk::Button m_nas_add, m_nas_del, m_index_add, m_index_del, m_flush;

    Gtk::OptionMenu m_OptionNASMenu, m_OptionIndexMenu;
    Gtk::Entry m_in_nas, m_in_index;
    Gtk::Label m_nas_remake, m_index_remake;
Gtk::Menu m_NAS_menu, m_Index_menu;
vector<string> m_curr_nas_v, m_curr_index_v;

MasterWin* pp;
db_config m_db;
};
#endif // _NASMASTER_H_
