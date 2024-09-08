#include "date_selector_ncurses.h"

DateSelectorNcurses::DateSelectorNcurses()
    : BarNCurses(WindowPosition::top, WindowFormat::half_line),
      range(Date(DatePoint::DayBegin), DatePoint::DayEnd) {};

DateSelectorNcurses::DateSelectorNcurses(const DateRange &date_range)
    : BarNCurses(WindowPosition::top, WindowFormat::half_line),
      range(date_range) {};

// TODO: This seems a bit wasteful. Store the string to display instead.
//       Add an update method if needed.
//       Is explicit printing at all needed after a clear?
//       Can't we just refresh?
void DateSelectorNcurses::refresh() const {
  BarNCurses::print(range.start.get_day_string());
}

void DateSelectorNcurses::clear() const { BarNCurses::clear(); }

DateRange DateSelectorNcurses::current_range() const { return range; }

void DateSelectorNcurses::select_next_day() { range.add_one_day(); }

void DateSelectorNcurses::select_previous_day() { range.subtract_one_day(); }
