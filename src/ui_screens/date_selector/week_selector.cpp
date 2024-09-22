#include "week_selector.h"

WeekSelector::WeekSelector()
    : BarNCurses(WindowPosition::top, WindowFormat::half_line), week{} {}

Week WeekSelector::current_week() const {
  return week;
}

DateRange WeekSelector::current_range() const { return week.to_date_range(); }

void WeekSelector::select_next_week() {
  week.next();
  this->refresh();
}

void WeekSelector::select_previous_week() {
  week.previous();
  this->refresh();
}

void WeekSelector::refresh() { BarNCurses::print(week.to_string()); }
