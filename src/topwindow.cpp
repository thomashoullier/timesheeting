#include "topwindow.h"

TopWindow::TopWindow() : m_VBox(Gtk::Orientation::VERTICAL)
{
  set_title(TIMESHEETING_TITLE);
  set_default_size(400, 150);

  // Populate widgets
  this->set_child(m_VBox);
  m_VBox.append(*(menubar.widget));

  m_VBox.append(tabs);
}
