#ifndef _BROWSEFOLDER_H_
#define _BROWSEFOLDER_H_
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <string>
using std::string;
class BrowseFolder : public Gtk::Dialog
{
public:
	BrowseFolder(Gtk::Frame& parent);
	virtual ~BrowseFolder();
	string get_selected();
protected:
	class ModelColumns : public Gtk::TreeModelColumnRecord
	{
	public:
	Gtk::TreeModelColumn<std::string> path;
	Gtk::TreeModelColumn<bool>   is_parent;

	ModelColumns() { add(path);add(is_parent); }
	};
	const ModelColumns m_columns;
	Glib::RefPtr<Gtk::ListStore> m_model;
private:
  void on_list_row_activated(const Gtk::TreeModel::Path& path,  Gtk::TreeViewColumn* _column);
  void on_ok_clicked();
  void show_folder_list();

  Gtk::VBox m_VBox;
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::ListViewText m_ListViewText;
  Gtk::VButtonBox m_ButtonBox;
  Gtk::Button m_Button_OK;
  Gtk::Label m_LablePath;
  string m_curr_path, m_root_path, m_selected;
};
#endif // _BROWSEFOLDER_H_
