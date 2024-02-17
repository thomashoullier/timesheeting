#include "examplewindow.h"

ExampleWindow::ExampleWindow() : m_VBox(Gtk::Orientation::VERTICAL),
  m_ButtonBox(Gtk::Orientation::VERTICAL),
  m_Button("Show AboutDialog")
{
  set_title(TIMESHEETING_TITLE);
  set_default_size(400, 150);

  set_child(m_VBox);

  m_VBox.append(m_ButtonBox);
  m_ButtonBox.append(m_Button);
  m_Button.set_expand(true);
  m_Button.set_halign(Gtk::Align::CENTER);
  m_Button.set_valign(Gtk::Align::CENTER);
  m_Button.signal_clicked().connect(sigc::mem_fun(*this,
              &ExampleWindow::on_button_clicked) );

  m_Dialog.set_transient_for(*this);
  m_Dialog.set_hide_on_close();

  //m_Button.grab_focus();
}

ExampleWindow::~ExampleWindow()
{
}

void ExampleWindow::on_button_clicked()
{
  m_Dialog.show();

  //Bring it to the front, in case it was already shown:
  //m_Dialog.present();
}
