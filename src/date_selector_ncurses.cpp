#include "date_selector_ncurses.h"
#include "bar_ncurses.h"
#include "date.h"
#include "win_ncurses.h"
#include <ncurses.h>

DateSelectorNcurses::DateSelectorNcurses()
  : BarNCurses(WindowPosition::top),
    range(Date(DatePoint::DayBegin), DatePoint::DayEnd) {};

void DateSelectorNcurses::refresh() const {
  BarNCurses::print(range.start.get_day_string());
}

void DateSelectorNcurses::clear() const { BarNCurses::clear(); }

DateRange DateSelectorNcurses::current_range() const { return range; }

void DateSelectorNcurses::select_next_day() { range.add_one_day(); }

void DateSelectorNcurses::select_previous_day() { range.subtract_one_day(); }
