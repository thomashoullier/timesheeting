#include "day_selector.h"
#include "ncurses_lib/win_ncurses.h"
// IWYU pragma: no_include <string>

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
    DaySelector::refresh();
  }

  void DaySelector::select_previous_day() {
    day.previous();
    DaySelector::refresh();
  }

  time_lib::Day DaySelector::get_selected_day() {
    return day;
  }
}
