#include "daily_entries.h"

DailyEntries::DailyEntries () {
  this->set_orientation(Gtk::Orientation::VERTICAL);
  this->append(cal_box);
  this->append(entries_list);
  // this->append(stopwatch);
}
