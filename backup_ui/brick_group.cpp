#include "brick_group.h"
#include "backup.h"
Brick_Ctrl::Brick_Ctrl(bool horizontal,
                       const Glib::ustring& title,
                       const Glib::ustring& _backup_path,
                       gint spacing,
                       Gtk::ButtonBoxStyle layout)
    : Gtk::Frame(title),
      m_bn_ctrl("START"),
      m_lbrick_id(title),
      m_lbrick_backup_path(_backup_path),
      m_msg(""),
      m_brick_id(title),
      m_stat(STOP)
{
    /*
    Gtk::ButtonBox* bbox = 0;

    // Ë®Æ½ »ò ÊúÖ±
    if(horizontal)
        bbox = Gtk::manage( new Gtk::HButtonBox() );
    else
        bbox = Gtk::manage( new Gtk::VButtonBox() );
        */
    Gtk::Box* pbox = 0;

    // Ë®Æ½ »ò ÊúÖ±
    if(horizontal)
        pbox = Gtk::manage( new Gtk::HBox() );
    else
        pbox = Gtk::manage( new Gtk::VBox() );
    pbox->set_border_width(5);
    pbox->set_homogeneous(false);
//    bbox->set_child_min_width(100);
    add(*pbox);
    /* Set the appearance of the Button Box */
    pbox->set_spacing(spacing);
    //pbox->pack_start(m_bn_ctrl, Gtk::PACK_SHRINK, 1);
    pbox->pack_start(m_bn_ctrl, Gtk::PACK_SHRINK, 5);
    pbox->pack_start(m_lbrick_id, Gtk::PACK_SHRINK, 5);
    pbox->pack_start(m_lbrick_backup_path, Gtk::PACK_SHRINK, 5);
    pbox->pack_start(m_msg, Gtk::PACK_SHRINK, 5);
    m_bn_ctrl.signal_clicked().connect(sigc::mem_fun(*this, &Brick_Ctrl::on_button_clicked));
}

void Brick_Ctrl::change_stat(bri_stat _s, const char* msg)
{
    switch(_s)
    {
        case BRI_START:
            m_bn_ctrl.set_label("STOP");
            m_stat = START;
            break;
        case BRI_STOP:
            m_bn_ctrl.set_label("START");
            m_stat = STOP;
            break;
        case BRI_MSG:
            if(msg != NULL)m_msg.set_label(msg);
            break;
        case BRI_ERR:
        {
            string msg_show = "<span foreground=\"#FF0000\">";
            if (msg != NULL) msg_show += msg;
            else msg_show += "ERR";
            msg_show += "</span>";
            m_msg.set_markup(msg_show.c_str());
        }
            break;
        default:
            break;
    }
}

void Brick_Ctrl::on_button_clicked()
{
    switch(m_stat)
    {
        case START:
            backup_stopbybrick(m_brick_id.c_str());
            break;
        case STOP:
            backup_startbybrick(m_brick_id.c_str());
            break;
        default:
            break;
    }

}
