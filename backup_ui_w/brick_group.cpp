#include "brick_group.h"
ExampleButtonBox::ExampleButtonBox(bool horizontal,
                                   const Glib::ustring& title,
                                   gint spacing,
                                   Gtk::ButtonBoxStyle layout)
    : Gtk::Frame(title),
      m_Button_Ctrl("START"),
      m_label(title)
 //   m_Button_OK("OK"),
 //   m_Button_Cancel("Cancel"),
 //   m_Button_Help("Help")
{
    Gtk::ButtonBox* bbox = 0;

    // 水平 或 竖直
    if(horizontal)
        bbox = Gtk::manage( new Gtk::HButtonBox() );
    else
        bbox = Gtk::manage( new Gtk::VButtonBox() );
    bbox->set_border_width(5);
    add(*bbox);
    /* Set the appearance of the Button Box */
    bbox->set_layout(layout);
    bbox->set_spacing(spacing);
    bbox->add(m_Button_Ctrl);
    bbox->add(m_label);
 //   bbox->add(m_Button_OK);
 //   bbox->add(m_Button_Cancel);
 //   bbox->add(m_Button_Help);
}
