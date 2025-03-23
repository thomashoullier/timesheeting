#include "day_selector.h"

namespace tui {
  DaySelector::DaySelector()
    : BarNCurses{ncurses_lib::WindowPosition::top,
                 ncurses_lib::WindowFormat::half_line}, day{} {
  }

  void DaySelector::refresh() {
    BarNCurses::print(day.to_string());
  }

  time_lib::DateRange DaySelector::current_range() const {
    return day.to_date_range();
  }

  void DaySelector::select_next_day() {
    day.next();
    this->refresh();
  }

  void DaySelector::select_previous_day() {
    day.previous();
    this->refresh();
  }

  time_lib::Day DaySelector::get_selected_day() {
    return day;
  }
}
