#include "BrickMaster.h"
#include "masterwin.h"
#include "BrowseFolder.h"
#include <iostream>
using namespace std;

BrickMaster::BrickMaster(const MasterWin* _pp):
m_add("ADD"),
m_del("DEL"),
m_check_date("CHECK"),
m_flush("FLUSH"),
m_browse_mount_path("BROWSE"),
m_browse_backup_path("BROWSE"),
m_LPos("pos"),
m_LBrickId("brick id"),
m_LPath("path"),
m_LMountPath("mount path"),
m_LBackPath("backup to"),
m_LBeginDate("start time(e.g. 1344585483)"),
m_LInterval("Interval:default 3600(1 hour)"),
m_date(""),
pp(const_cast<MasterWin*>(_pp))
{
    add(m_main);
    m_main.set_border_width(10);
    Gtk::Box* pcombox = Gtk::manage( new Gtk::HBox() );
    pcombox->pack_start(m_flush, Gtk::PACK_SHRINK, 1);
    m_main.pack_start(*pcombox, Gtk::PACK_SHRINK, 5);
    m_flush.signal_clicked().connect(sigc::mem_fun(*this, &BrickMaster::on_flush));
    m_add.signal_clicked().connect(sigc::mem_fun(*this, &BrickMaster::on_add));
    m_del.signal_clicked().connect(sigc::mem_fun(*this, &BrickMaster::on_del));
    m_check_date.signal_clicked().connect(sigc::mem_fun(*this, &BrickMaster::on_check_date));
    m_browse_mount_path.signal_clicked().connect(sigc::mem_fun(*this, &BrickMaster::on_select_mount_path));
    m_browse_backup_path.signal_clicked().connect(sigc::mem_fun(*this, &BrickMaster::on_select_backup_path));
    on_flush();

    Gtk::Box* pbox_2 = Gtk::manage( new Gtk::HBox() );
    m_main.pack_start(*pbox_2 , Gtk::PACK_SHRINK, 5);
    m_OptionBrickMenu.set_menu(m_Brick_menu);
    pbox_2->pack_start(m_OptionBrickMenu , Gtk::PACK_SHRINK, 5);
    pbox_2->pack_start(m_del , Gtk::PACK_SHRINK, 5);

    Gtk::Box* pbox_3 = Gtk::manage( new Gtk::HBox() );
    m_main.pack_start(*pbox_3 , Gtk::PACK_SHRINK, 5);
    m_OptionNASMenu.set_menu(m_NAS_menu);
    m_OptionIndexMenu.set_menu(m_Index_menu);
    pbox_3->pack_start(m_OptionNASMenu , Gtk::PACK_SHRINK, 5);
    pbox_3->pack_start(m_OptionIndexMenu , Gtk::PACK_SHRINK, 5);
    //pbox_3->pack_start(m_EntryDiskPos , Gtk::PACK_SHRINK, 5);         // disk pos for brick
    pbox_3->pack_start(m_LPos , Gtk::PACK_SHRINK, 5);

    Gtk::Box* pbox_4 = Gtk::manage( new Gtk::HBox() );
    m_main.pack_start(*pbox_4 , Gtk::PACK_SHRINK, 5);
    Gtk::Box* pboxl_1 = Gtk::manage( new Gtk::VBox() );
    Gtk::Box* pboxl_2 = Gtk::manage( new Gtk::VBox() );
    Gtk::Box* pboxl_3 = Gtk::manage( new Gtk::VBox() );
    Gtk::Box* pboxl_4 = Gtk::manage( new Gtk::VBox() );
    pbox_4->pack_start(*pboxl_1 , Gtk::PACK_SHRINK, 5);
    pbox_4->pack_start(*pboxl_2 , Gtk::PACK_SHRINK, 5);
    pbox_4->pack_start(*pboxl_3 , Gtk::PACK_SHRINK, 5);
    pbox_4->pack_start(*pboxl_4 , Gtk::PACK_SHRINK, 5);
    // first
//Gtk::OptionMenu m_OptionNASMenu, m_OptionIndexMenu, m_OptionBrickMenu;
//Gtk::Menu m_NAS_menu, m_Index_menu, m_Brick_menu;
/*
    m_OptionBrickMenu.set_menu(m_Brick_menu);
    pboxl_1->pack_start(m_OptionBrickMenu , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_del , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);

    // second
    m_OptionNASMenu.set_menu(m_NAS_menu);
    m_OptionIndexMenu.set_menu(m_Index_menu);
    pboxl_1->pack_start(m_OptionNASMenu , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_OptionIndexMenu , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(m_EntryDiskPos , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(m_LPos , Gtk::PACK_EXPAND_WIDGET, 5);
  */
    pboxl_1->pack_start(m_EntryBrickId, Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_LBrickId, Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);
    // third
    pboxl_1->pack_start(m_EntryPath , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_LPath , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);
    // fourth
    pboxl_1->pack_start(m_EntryMountPath , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_LMountPath , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(m_browse_mount_path , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);
    // fifth
    pboxl_1->pack_start(m_EntryBackPath , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_LBackPath , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(m_browse_backup_path , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);
    // sixth
    pboxl_1->pack_start(m_EntryBeginDate , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_LBeginDate , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(m_check_date , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(m_date , Gtk::PACK_EXPAND_WIDGET, 5);
    // seventh
    pboxl_1->pack_start(m_EntryInterval , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(m_LInterval , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(*Gtk::manage(new Gtk::Label()), Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(*Gtk::manage(new Gtk::Label()), Gtk::PACK_EXPAND_WIDGET, 5);
    // eighth
    pboxl_1->pack_start(*Gtk::manage(new Gtk::Label()), Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_2->pack_start(*Gtk::manage(new Gtk::Label()), Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_3->pack_start(m_add , Gtk::PACK_EXPAND_WIDGET, 5);
    pboxl_4->pack_start(*Gtk::manage(new Gtk::Label()) , Gtk::PACK_EXPAND_WIDGET, 5);

}

BrickMaster::~BrickMaster()
{

}

void BrickMaster::on_flush()
{
    using namespace Gtk::Menu_Helpers;
    /*
    MenuList& list_naspos = m_NAS_menu.items();
    list_naspos.clear();
    m_curr_nas_v.clear();
    list_naspos.push_back(
        MenuElem("NAS-1"));
    list_naspos.push_back(
        MenuElem("NAS-2"));
    m_OptionNASMenu.set_history(0);
    m_curr_nas_v.push_back("NAS-1");
    m_curr_nas_v.push_back("NAS-2");
    // query index info
    MenuList& list_indexpos = m_Index_menu.items();
    list_indexpos.clear();
    m_curr_index_v.clear();
    list_indexpos.push_back(
        MenuElem("3"));
    list_indexpos.push_back(
        MenuElem("4"));
    m_curr_index_v.push_back("3");
    m_curr_index_v.push_back("4");
    m_OptionIndexMenu.set_history(0);

    MenuList& list_brickpos = m_Brick_menu.items();
    list_brickpos.clear();
    m_curr_brick_v.clear();
    list_brickpos.push_back(
        MenuElem("brick-1"));
    list_brickpos.push_back(
        MenuElem("brick-2"));
    m_curr_brick_v.push_back("brick-1");
    m_curr_brick_v.push_back("brick-2");
    m_OptionBrickMenu.set_history(0);
*/
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

    MenuList& list_brickpos = m_Brick_menu.items();
    list_brickpos.clear();
    m_curr_brick_v.clear();
    m_db.query_brick_id(m_curr_brick_v);
    for(vector<string>::iterator item = m_curr_brick_v.begin(); item != m_curr_brick_v.end(); ++item)
    {
            list_brickpos.push_back(MenuElem((*item).c_str()));
    }
    m_OptionBrickMenu.set_history(0);
}
void BrickMaster::on_add()
{
	// check
	if (m_curr_nas_v.size() == 0	||
		m_curr_index_v.size() == 0	||
		m_EntryBrickId.get_text().empty() ||
		m_EntryPath.get_text().empty()    ||
		m_EntryMountPath.get_text().empty()||
		m_EntryBackPath.get_text().empty() ||
		m_EntryBeginDate.get_text().empty() ||
		m_EntryInterval.get_text().empty()
		)
	{
        pp->showmsg("brick info is not enough", true);
		return;
	}

	// format time
	time_t tmp = atoll(m_EntryBeginDate.get_text().c_str());
	if (tmp <= 0)
	{
		m_EntryBeginDate.set_text("0");
	}

	if (m_EntryBeginDate.get_text().size() > 10 ||
		m_EntryBeginDate.get_text() > "4294967295")
	{
		m_EntryBeginDate.set_text("4294967295");
	}
	tmp = atoll(m_EntryBeginDate.get_text().c_str());

    cout << m_curr_nas_v[m_OptionNASMenu.get_history()] << "-" << m_curr_index_v[m_OptionIndexMenu.get_history()] << "-"
         << m_EntryBrickId.get_text() << "-" << m_EntryPath.get_text() << "-" << m_EntryMountPath.get_text() << "-"
         << m_EntryBackPath.get_text() << "-" << m_EntryBeginDate.get_text() << "-" << m_EntryInterval.get_text() << endl;
    int checkret = m_db.check_nas_index(m_curr_nas_v[m_OptionNASMenu.get_history()], m_curr_index_v[m_OptionIndexMenu.get_history()]);
    if(checkret == BK_DB_ERROR)
    {
        pp->showmsg("check nas index error", true);
        return;
    }
    if(checkret == BK_DB_EXIST)
    {
        pp->showmsg("cann't add brick to this index of the nas", true);
        return;
    }
    if (m_db.add_brick(m_curr_nas_v[m_OptionNASMenu.get_history()],
                         m_curr_index_v[m_OptionIndexMenu.get_history()],
                         m_EntryBrickId.get_text(),
                         m_EntryPath.get_text(),
                         m_EntryMountPath.get_text(),
                         m_EntryBackPath.get_text(),
                         m_EntryBeginDate.get_text(),
                         m_EntryInterval.get_text()) != BK_DB_SUCESS)
                         {
                            pp->showmsg("brick id exist", true);
                         }
                         else
                         {
                             pp->showmsg("add brick sucess");
                             on_flush();
                         }
}
void BrickMaster::on_del()
{
	if (m_curr_brick_v.size() == 0)
	{
        pp->showmsg("no brick", true);
		return;
	}
    cout << m_curr_brick_v[m_OptionBrickMenu.get_history()] << endl;
    if (m_db.del_brick(m_curr_brick_v[m_OptionBrickMenu.get_history()]) != BK_DB_SUCESS)
    {
        pp->showmsg("delete brick failed, please check no data dependent on it", true);
    }
    else
    {
        pp->showmsg("delete brick sucess");
        on_flush();
    }
}
void BrickMaster::on_check_date()
{
	// format time
	time_t tmp = atoll(m_EntryBeginDate.get_text().c_str());
	if (tmp <= 0)
	{
		m_EntryBeginDate.set_text("0");
	}

	if (m_EntryBeginDate.get_text().size() > 10 ||
		m_EntryBeginDate.get_text() > "4294967295")
	{
		m_EntryBeginDate.set_text("4294967295");
	}
	tmp = atoll(m_EntryBeginDate.get_text().c_str());


    m_date.set_label(ctime(&tmp));
}

void BrickMaster::on_select_mount_path()
{
	BrowseFolder* pDialog = new BrowseFolder(*this);
	pDialog->run();
	m_EntryMountPath.set_text(pDialog->get_selected());
	delete pDialog;
}

void BrickMaster::on_select_backup_path()
{
	BrowseFolder* pDialog = new BrowseFolder(*this);
	pDialog->run();
	m_EntryBackPath.set_text(pDialog->get_selected());
	delete pDialog;
}