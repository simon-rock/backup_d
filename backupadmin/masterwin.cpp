#include "masterwin.h"


MasterWin::MasterWin():
m_brick(this),
m_nas(this),
m_msg("show message")
{

set_title("surdoc backup admin " VERSION);
add(m_vmain);

m_refTextBuffer3 = Gtk::TextBuffer::create();
m_refTextBuffer3->set_text("query");

m_query.set_buffer(m_refTextBuffer3);

m_vmain.pack_start(m_msg, Gtk::PACK_SHRINK, 5);
//Notebook:
   m_Notebook.append_page(m_nas, "nas master", true); //true = use mnemonic.
   m_Notebook.append_page(m_brick, "brick master", true); //true = use mnemonic.
//   m_Notebook.append_page(m_query, "query", true); //true = use mnemonic.

m_vmain.pack_start(m_Notebook);

set_default_size (600, 400);

show_all();
}
MasterWin::~MasterWin()
{

}

void MasterWin::showmsg(const string& _msg, bool err)
{
    if(err)
    {
        string msg_show = "<span foreground=\"#FF0000\">";
        if (!_msg.empty()) msg_show += _msg;
        else msg_show += "ERR";
        msg_show += "</span>";
        m_msg.set_markup(msg_show.c_str());
    }
    else
    {
        m_msg.set_label(_msg);
    }
}
