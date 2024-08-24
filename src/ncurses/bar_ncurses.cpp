#include "bar_ncurses.h"
#include "win_ncurses.h"
#include <stdexcept>

BarNCurses::BarNCurses(WindowPosition winpos, WindowFormat winformat)
  : WinNCurses(winpos, filter_window_format(winformat)) {}

void BarNCurses::print(const std::string &msg) const {
  this->clear();
  wmove(win, 0, 0);
  wprintw(win, msg.c_str());
  this->refresh();
}

void BarNCurses::print_right(const std::string &msg) const {
  this->clear();
  int str_len = msg.length();
  int max_x = getmaxx(win);
  wmove(win, 0, max_x - str_len);
  wprintw(win, msg.c_str());
  this->refresh();
}

WindowFormat BarNCurses::filter_window_format(WindowFormat winformat) {
  switch(winformat) {
  case WindowFormat::line:
    return winformat;
  case WindowFormat::half_line:
    return winformat;
  default:
    throw std::logic_error("WindowFormat is incompatible with BarNCurses.");
  }
}
