#include "menubar.h"

MenuBar::MenuBar() {
  auto m_refBuilder = Gtk::Builder::create();

  auto m_refActionGroup = Gio::SimpleActionGroup::create();
  m_refActionGroup->add_action(
      "about", sigc::mem_fun(*this, &MenuBar::about_on_button_clicked));

  const Glib::ustring ui_info =
      "<interface>"
      "  <menu id='menubar'>"
      "    <submenu>"
      "      <attribute name='label'>_File</attribute>"
      "      <section>"
      "        <item>"
      "          <attribute name='label'>_Open</attribute>"
      "          <attribute name='action'>menu.open</attribute>"
      "        </item>"
      "        <item>"
      "          <attribute name='label'>_Save as</attribute>"
      "          <attribute name='action'>menu.save_as</attribute>"
      "        </item>"
      "      </section>"
      "    </submenu>"
      "    <submenu>"
      "      <attribute name='label'>_About</attribute>"
      "      <section>"
      "        <item>"
      "          <attribute name='label'>_About</attribute>"
      "          <attribute name='action'>menu.about</attribute>"
      "        </item>"
      "      </section>"
      "    </submenu>"
      "  </menu>"
      "</interface>";

  m_refBuilder->add_from_string(ui_info);

  auto gmenu = m_refBuilder->get_object<Gio::Menu>("menubar");
  this->set_menu_model(gmenu);
  this->insert_action_group("menu", m_refActionGroup);
}

void MenuBar::about_on_button_clicked() {
  about_dialog.show();
}
