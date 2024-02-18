#include "topwindow.h"

TopWindow::TopWindow() : m_VBox(Gtk::Orientation::VERTICAL) {
  set_title(TIMESHEETING_TITLE);
  set_default_size(400, 150);

  // Status bar at bottom.
  auto status_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
  auto db_state = Gtk::make_managed<Gtk::Label>("DB");
  auto stopwatch_state = Gtk::make_managed<Gtk::Label>("STP");
  status_box->append(*db_state);
  status_box->append(*stopwatch_state);
  status_box->set_halign(Gtk::Align::END);
  status_box->set_spacing(10);
  status_box->set_margin_end(10);

  // Populate widgets
  this->set_child(m_VBox);
  m_VBox.append(menubar);
  m_VBox.append(tabs);
  m_VBox.append(*status_box);
}
