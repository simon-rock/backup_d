#ifndef _MASTERWIN_H_
#define _MASTERWIN_H_

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
//#include <gtkmm/image.h>
#include <gtkmm/button.h>
//#include <gtkmm/stockid.h>
//#include <gtkmm/stock.h>
//#include <gtkmm/enums.h>
#include "BrickMaster.h"
#include "NASMaster.h"
class MasterWin : public Gtk::Window
{
public:
MasterWin();
virtual ~MasterWin();
void showmsg(const string& _msg, bool err = false);
protected:


//Signal handlers:


//Member widgets:
//WidebrightTab m_Notebook;
Gtk::Notebook m_Notebook;
Gtk::VBox m_vmain;

Gtk::TextView m_query;
Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer3;
BrickMaster m_brick;
NASMaster m_nas;

std::string title;
Gtk::Label m_msg;
};

#endif //_MASTERWIN_H_
