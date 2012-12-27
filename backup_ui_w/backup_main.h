#ifndef _BACKUP_MAIN_H_
#define _BACKUP_MAIN_H_
#include <gtkmm.h>
#include <string>
using std::string;
class ExampleWindow : public Gtk::Window
{
public:
    ExampleWindow();
    virtual ~ExampleWindow();
protected:
//Signal handlers:
    virtual void on_button_clicked();


//Child widgets:
    Gtk::VBox m_VBox_Main, m_VBox;
    Gtk::HBox m_HBox;
    Gtk::Frame m_Frame_Horizontal, m_Frame_Vertical;

private:
    static void * dispose_msg(void* _pPara);
    int add_backup(string _brick_id, string _brick_backup_path);
    pthread_t m_hthread;
    bool m_init;            // whether init complete
};
#endif //_BACKUP_MAIN_H_
