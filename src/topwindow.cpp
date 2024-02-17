#include "topwindow.h"

TopWindow::TopWindow() : m_VBox(Gtk::Orientation::VERTICAL)
{
  set_title(TIMESHEETING_TITLE);
  set_default_size(400, 150);

  // Populate widgets
  this->set_child(m_VBox);
  m_VBox.append(*(menubar.widget));

  auto m_Label1 = Gtk::make_managed<Gtk::Label>("Contents of tab 1");
  auto m_Label2 = Gtk::make_managed<Gtk::Label>("Contents of tab 2");

  m_Notebook.append_page(*m_Label1, "First");
  m_Notebook.append_page(*m_Label2, "Second");
  m_Notebook.set_expand(true);
  m_VBox.append(m_Notebook);
}
