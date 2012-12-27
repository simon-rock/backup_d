#ifndef _BRICK_GROUP_H
#define _BRICK_GROUP_H
#include <gtkmm.h>
class ExampleButtonBox : public Gtk::Frame
{
public:
    ExampleButtonBox(bool horizontal,
                     const Glib::ustring& title,
                     gint spacing,
                     Gtk::ButtonBoxStyle layout);
protected:
    Gtk::Button m_Button_Ctrl;//, m_Button_Cancel, m_Button_Help;
    Gtk::Label m_label;
};
#endif //_BRICK_GROUP_H
