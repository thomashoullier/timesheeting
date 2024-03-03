#include "calbox.h"
#include "glibmm/date.h"

CalBox::CalBox() {
  this->set_orientation(Gtk::Orientation::HORIZONTAL);
  this->set_center_widget(cal_day);
  this->set_start_widget(prev_day);
  this->set_end_widget(next_day);

  // Placeholder day
  auto day_str = Glib::Date(Glib::Date::Day(2), Glib::Date::Month(3),
                            Glib::Date::Year(2024));
  cal_day.set_label(day_str.format_string("%a, %d %b %Y"));
  // Arrows
  prev_day.set_icon_name("go-previous");
  next_day.set_icon_name("go-next");
}
