#include "topwindow.h"
#include "giomm/simpleactiongroup.h"
#include "gtkmm/popovermenu.h"
#include "gtkmm/popovermenubar.h"

TopWindow::TopWindow() : m_VBox(Gtk::Orientation::VERTICAL),
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
              &TopWindow::on_button_clicked) );

  about_dialog.set_transient_for(*this);
  about_dialog.set_hide_on_close();

  // Menu actions
  auto m_refActionGroup = Gio::SimpleActionGroup::create();
  m_refActionGroup->add_action(
      "about", sigc::mem_fun(*this, &TopWindow::on_button_clicked));
  insert_action_group("example", m_refActionGroup);

  // Menu objects
  auto m_refBuilder = Gtk::Builder::create();

  const Glib::ustring ui_info =
  "<interface>"
  "  <menu id='menubar'>"
  "    <submenu>"
  "      <attribute name='label'>_About</attribute>"
  "      <section>"
  "        <item>"
  "          <attribute name='label' translatable='yes'>_About</attribute>"
  "          <attribute name='action'>example.about</attribute>"
  "        </item>"
  "      </section>"
  "    </submenu>"
  "  </menu>"
  "</interface>";

  m_refBuilder->add_from_string(ui_info);

  auto gmenu = m_refBuilder->get_object<Gio::Menu>("menubar");
  auto pMenuBar = Gtk::make_managed<Gtk::PopoverMenuBar>(gmenu);
  m_VBox.prepend(*pMenuBar);
}

void TopWindow::on_button_clicked()
{
  about_dialog.show();
}
