#include "NASMaster.h"
#include "masterwin.h"
#include <iostream>
using namespace std;
NASMaster::NASMaster(const MasterWin* _pp):
m_nas_add("ADD"),
m_nas_del("DEL"),
m_index_add("ADD"),
m_index_del("DEL"),
m_flush("FLUSH"),
m_nas_remake("nas name"),
m_index_remake("max brick number(must be numeric)"),
pp(const_cast<MasterWin*>(_pp))
{
    add(m_main);
    m_main.set_border_width(10);
    Gtk::Box* pcombox = Gtk::manage( new Gtk::HBox() );
    pcombox->pack_start(m_flush, Gtk::PACK_SHRINK, 1);
    m_main.pack_start(*pcombox, Gtk::PACK_SHRINK, 5);
    m_flush.signal_clicked().connect(sigc::mem_fun(*this, &NASMaster::on_flush));
    m_nas_add.signal_clicked().connect(sigc::mem_fun(*this, &NASMaster::on_add_nas));
    m_nas_del.signal_clicked().connect(sigc::mem_fun(*this, &NASMaster::on_del_nas));
    m_index_add.signal_clicked().connect(sigc::mem_fun(*this, &NASMaster::on_add_index));
    m_index_del.signal_clicked().connect(sigc::mem_fun(*this, &NASMaster::on_del_index));

    on_flush();
    // nas
    Gtk::Box* pnasbox = 0;
    pnasbox = Gtk::manage( new Gtk::HBox() );
    pnasbox->set_border_width(1);
    pnasbox->set_homogeneous(false);
    m_OptionNASMenu.set_menu(m_NAS_menu);
    pnasbox->pack_start(m_in_nas, Gtk::PACK_SHRINK, 5);
    pnasbox->pack_start(m_nas_add, Gtk::PACK_SHRINK, 5);
    pnasbox->pack_start(m_OptionNASMenu, Gtk::PACK_SHRINK, 5);
    pnasbox->pack_start(m_nas_del, Gtk::PACK_SHRINK, 5);
    pnasbox->pack_start(m_nas_remake, Gtk::PACK_SHRINK, 5);
    m_nas.pack_start(*pnasbox, Gtk::PACK_EXPAND_WIDGET, 5);
    m_nas.set_border_width(5);
    m_main.pack_start(m_nas, Gtk::PACK_SHRINK, 5);

    // brick index
    Gtk::Box* pindexbox = 0;
    pindexbox = Gtk::manage( new Gtk::HBox() );
    pindexbox->set_border_width(1);
    pindexbox->set_homogeneous(false);

    m_OptionIndexMenu.set_menu(m_Index_menu);
    pindexbox->pack_start(m_in_index, Gtk::PACK_SHRINK, 5);
    pindexbox->pack_start(m_index_add, Gtk::PACK_SHRINK, 5);
    pindexbox->pack_start(m_OptionIndexMenu, Gtk::PACK_SHRINK, 5);
    pindexbox->pack_start(m_index_del, Gtk::PACK_SHRINK, 5);
    pindexbox->pack_start(m_index_remake, Gtk::PACK_SHRINK, 5);
    m_brick_index.pack_start(*pindexbox, Gtk::PACK_EXPAND_WIDGET, 5);
    m_brick_index.set_border_width(5);
    m_main.pack_start(m_brick_index, Gtk::PACK_SHRINK, 5);
}

NASMaster::~NASMaster()
{

}

void NASMaster::on_flush()
{
    // query nas
    using namespace Gtk::Menu_Helpers;
    MenuList& list_naspos = m_NAS_menu.items();
    list_naspos.clear();
    m_curr_nas_v.clear();
    m_db.query_nas(m_curr_nas_v);
    for(vector<string>::iterator item = m_curr_nas_v.begin(); item != m_curr_nas_v.end(); ++item)
    {
            list_naspos.push_back(MenuElem((*item).c_str()));
    }
    m_OptionNASMenu.set_history(0);

    MenuList& list_indexpos = m_Index_menu.items();
    list_indexpos.clear();
    m_curr_index_v.clear();
    m_db.query_index(m_curr_index_v);
    for(vector<string>::iterator item = m_curr_index_v.begin(); item != m_curr_index_v.end(); ++item)
    {
            list_indexpos.push_back(MenuElem((*item).c_str()));
    }
    m_OptionIndexMenu.set_history(0);
}
void NASMaster::on_add_nas()
{
	if(m_in_nas.get_text().size() == 0)
	{
		pp->showmsg("nas name can not be empty", true);
		return;
	}
    if (m_db.add_nas(m_in_nas.get_text()) != BK_DB_SUCESS)
    {
        pp->showmsg("add nas failed, nas id exist", true);
    }
    else
    {
        pp->showmsg("add nas sucess");
        on_flush();
    }
    cout << m_in_nas.get_text()<< endl;
}
void NASMaster::on_del_nas()
{
	if(m_curr_nas_v.size() == 0)
	{
		pp->showmsg("no nas name", true);
		return;
	}
    if(m_db.del_nas(m_curr_nas_v[m_OptionNASMenu.get_history()]) != BK_DB_SUCESS)
    {
        pp->showmsg("delete nas failed, please check no data dependent on it", true);
    }
    else
    {
        pp->showmsg("delete nas sucess");
        on_flush();
    }
}
void NASMaster::on_add_index()
{
	if(m_in_index.get_text().size() == 0)
	{
		pp->showmsg("index can not be empty", true);
		return;
	}
    if (m_db.add_index(m_in_index.get_text()) != BK_DB_SUCESS)
    {
        pp->showmsg("add indes failed, indes exist", true);
    }
    else
    {
        pp->showmsg("add indes sucess");
        on_flush();
    }
//        Gtk::OptionMenu m_OptionNASMenu, m_OptionIndexMenu;
    cout << m_in_index.get_text()<< endl;
}
void NASMaster::on_del_index()
{
	if(m_curr_index_v.size() == 0)
	{
		pp->showmsg("no index", true);
		return;
	}
    if(m_db.del_index(m_curr_index_v[m_OptionIndexMenu.get_history()]) != BK_DB_SUCESS)
    {
        pp->showmsg("delete index failed, please check no data dependent on it", true);
    }
    else
    {
        pp->showmsg("delete index sucess");
        on_flush();
    }
}

















