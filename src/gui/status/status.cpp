#include "status.h"

Status::Status() {
  this->set_orientation(Gtk::Orientation::HORIZONTAL);
  this->set_halign(Gtk::Align::END);
  this->set_spacing(10);
  this->set_margin_end(10);

  // Child widgets
  auto db_state = Gtk::make_managed<Gtk::Label>("DB");
  auto stopwatch_state = Gtk::make_managed<Gtk::Label>("STP");
  this->append(*db_state);
  this->append(*stopwatch_state);
}
