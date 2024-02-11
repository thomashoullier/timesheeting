#include "examplewindow.h"

ExampleWindow::ExampleWindow() : m_VBox(Gtk::Orientation::VERTICAL),
  m_Label("The AboutDialog is non-modal. "
    "You can select parts of this text while the AboutDialog is shown."),
  m_ButtonBox(Gtk::Orientation::VERTICAL),
  m_Button("Show AboutDialog")
{
  set_title("Gtk::AboutDialog example");
  set_default_size(400, 150);

  set_child(m_VBox);

  m_VBox.append(m_Label);
  m_Label.set_expand(true);
  m_Label.set_wrap(true);
  m_Label.set_selectable(true);

  m_VBox.append(m_ButtonBox);
  m_ButtonBox.append(m_Button);
  m_Button.set_expand(true);
  m_Button.set_halign(Gtk::Align::CENTER);
  m_Button.set_valign(Gtk::Align::CENTER);
  m_Button.signal_clicked().connect(sigc::mem_fun(*this,
              &ExampleWindow::on_button_clicked) );

  m_Dialog.set_transient_for(*this);
  m_Dialog.set_hide_on_close();

  m_Dialog.set_program_name("Example application");
  m_Dialog.set_version("1.0.0");
  m_Dialog.set_copyright("Murray Cumming");
  m_Dialog.set_comments("This is just an example application.");
  m_Dialog.set_license("LGPL");

  m_Dialog.set_website("http://www.gtkmm.org");
  m_Dialog.set_website_label("gtkmm website");

  std::vector<Glib::ustring> list_authors;
  list_authors.push_back("Murray Cumming");
  list_authors.push_back("Somebody Else");
  list_authors.push_back("AN Other");
  m_Dialog.set_authors(list_authors);

  m_Button.grab_focus();
}

ExampleWindow::~ExampleWindow()
{
}

void ExampleWindow::on_button_clicked()
{
  m_Dialog.show();

  //Bring it to the front, in case it was already shown:
  m_Dialog.present();
}
