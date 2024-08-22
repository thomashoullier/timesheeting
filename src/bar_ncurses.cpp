#include "bar_ncurses.h"

void BarNCurses::print(const std::string &msg) const {
  this->clear();
  wmove(win, 0, 0);
  wprintw(win, msg.c_str());
  this->refresh();
}
