#include "date_selector_ncurses.h"
#include "date.h"
#include <ncurses.h>

DateSelectorNcurses::DateSelectorNcurses()
    : range(Date(DatePoint::DayBegin), DatePoint::DayEnd),
      win(init_window()) {};

DateSelectorNcurses::~DateSelectorNcurses() {
  delwin(win);
}

void DateSelectorNcurses::print() const {
  wmove(win, 0, 0);
  wclrtoeol(win);
  auto day_str = range.start.get_day_string();
  wprintw(win, day_str.c_str());
  wrefresh(win);
}

void DateSelectorNcurses::refresh() const {
  this->print();
}

void DateSelectorNcurses::clear() const {
  wclear(win);
  wrefresh(win);
}

DateRange DateSelectorNcurses::current_range() const {
  return range;
}

void DateSelectorNcurses::select_next_day() {
  range.start.add_one_day();
  range.stop.add_one_day();
}

void DateSelectorNcurses::select_previous_day() {
  range.start.subtract_one_day();
  range.stop.subtract_one_day();
}

WINDOW *DateSelectorNcurses::init_window() {
  auto max_x = getmaxx(stdscr);
  return newwin(1, max_x, 1, 0);
}
