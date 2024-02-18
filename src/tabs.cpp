#include "tabs.h"

Tabs::Tabs () {
  auto m_Label1 = Gtk::make_managed<Gtk::Label>("Contents of tab 1");
  auto m_Label2 = Gtk::make_managed<Gtk::Label>("Contents of tab 2");

  this->append_page(*m_Label1, "First");
  this->append_page(*m_Label2, "Second");
  this->set_expand(true);
}
