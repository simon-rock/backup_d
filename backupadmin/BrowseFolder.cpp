#include "BrowseFolder.h"

BrowseFolder::BrowseFolder(Gtk::Frame& parent)
	: Gtk::Dialog("Interactive Dialog", NULL, true),
    m_VBox(false, 8),
    m_ListViewText(1),
    m_Button_OK("OK"),
	m_LablePath("/"),
    m_curr_path("/"),
    m_root_path("/")
{
    set_title("browse folder");
    set_border_width(1);
    set_default_size(400, 200);

    get_vbox()->pack_start(m_VBox, Gtk::PACK_EXPAND_WIDGET);

    m_ScrolledWindow.add(m_ListViewText);
    m_ScrolledWindow.set_size_request(300, 200);

    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_VBox.pack_start(m_LablePath, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_ScrolledWindow, Gtk::PACK_EXPAND_WIDGET);
    m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

    m_ButtonBox.pack_start(m_Button_OK, Gtk::PACK_SHRINK);
    m_ButtonBox.set_border_width(1);
    m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
    m_Button_OK.signal_clicked().connect( sigc::mem_fun(*this,
                                          &BrowseFolder::on_ok_clicked) );

    m_ListViewText.set_column_title(0, "path");

    m_ListViewText.signal_row_activated().connect( sigc::mem_fun(*this, &BrowseFolder::on_list_row_activated) );
    m_model = Gtk::ListStore::create(m_columns);
    m_ListViewText.set_model(m_model);
    show_folder_list();

    show_all_children();
}

BrowseFolder::~BrowseFolder()
{
}

void BrowseFolder::on_ok_clicked()
{
    Glib::RefPtr<Gtk::TreeSelection> refSelection = m_ListViewText.get_selection();
    if(const Gtk::TreeModel::iterator iter = refSelection->get_selected())
    {
        const Gtk::TreeModel::Path path(iter);

        if(iter)
        {
            Gtk::TreeModel::Row row = *iter;
            const bool is_parent = row[m_columns.is_parent];
            const std::string filepath = row[m_columns.path];
            if(is_parent)
            {
            }
            else
            {
                m_selected = filepath;
            }
        }
    }
    hide();
}

void BrowseFolder::on_list_row_activated(const Gtk::TreeModel::Path& path,  Gtk::TreeViewColumn* _column)
{
    Gtk::TreeModel::iterator iter = m_model->get_iter(path);
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        const bool is_parent = row[m_columns.is_parent];
        const std::string filepath = row[m_columns.path];
        if(is_parent)
        {
            	string tmp  = m_curr_path;
                string::size_type index = tmp.rfind('/');
                if(index != string::npos && index > m_root_path.size())
                {
                    tmp = tmp.substr(0, tmp.rfind('/'));
                    index = tmp.rfind('/');
                    if(index != string::npos && index + 1 >= m_root_path.size())
                    {
                        tmp = tmp.substr(0, tmp.rfind('/') + 1);
                    }
                }
                m_curr_path = tmp;
				m_LablePath.set_text(m_curr_path);
        }
        else
        {
            m_curr_path += filepath;
            m_curr_path += "/";
			m_LablePath.set_text(m_curr_path);
        }
        show_folder_list();
    }
}

string BrowseFolder::get_selected()
{
	if (m_selected.empty())
	{
	    return m_curr_path;		
	}
	else
	{
	    return m_curr_path + m_selected + "/";
	}

}

void BrowseFolder::show_folder_list()
{
    m_model->clear();
    Glib::Dir dirs(m_curr_path);
    std::list<std::string> entries (dirs.begin(), dirs.end());
    for(std::list<std::string>::iterator  dirItr=entries.begin();dirItr!=entries.end();dirItr++)
    {
        Glib::ustring fullpath=m_curr_path+*dirItr;
//        std::cout<<"curr:"<<fullpath.c_str()<< std::endl;
        try
        {
           if (Glib::file_test(fullpath,Glib::FILE_TEST_IS_DIR))
            {
                Gtk::TreeModel::iterator iter = m_model->append();
                Gtk::TreeModel::Row row = *iter;
                row[m_columns.path] = *dirItr;
                row[m_columns.is_parent] = 0;
            }
        }
        catch(Glib::FileError er)
        {
//            std::cout<<"Error:"<<fullpath.c_str()<<":"<<er.what().c_str() <<std::endl;
        }
    }
    if(m_curr_path != m_root_path)
    {
        Gtk::TreeModel::iterator iter = m_model->append();
        Gtk::TreeModel::Row row = *iter;
        row[m_columns.path] = "..";
        row[m_columns.is_parent] = 1;
    }

}