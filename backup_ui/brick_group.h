#ifndef _BRICK_GROUP_H
#define _BRICK_GROUP_H
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <string>
using std::string;
class Brick_Ctrl : public Gtk::Frame
{
public:
    enum bri_stat
    {
        BRI_START = 0,
        BRI_STOP,
        BRI_MSG,
        BRI_ERR
    };

    Brick_Ctrl(bool horizontal,
                     const Glib::ustring& title,
                     const Glib::ustring& _backup_path,
                     gint spacing,
                     Gtk::ButtonBoxStyle layout);
    string get_id(){return m_brick_id;}
    void change_stat(bri_stat _s, const char* msg = NULL);
    void on_button_clicked();
protected:
    enum bk_stat
    {
        START = 0,
        STOP
    };
    Gtk::Button m_bn_ctrl;//, m_Button_Cancel, m_Button_Help;
    Gtk::Label m_lbrick_id, m_lbrick_backup_path, m_msg;
    string m_brick_id;
    bk_stat m_stat;
};
#endif //_BRICK_GROUP_H
