#include "status_bar_ncurses.h"
#include <curses.h>

StatusBarNCurses::StatusBarNCurses() : BarNCurses(WindowPosition::bottom) {}

void StatusBarNCurses::print(const std::string &msg) { BarNCurses::print(msg); }

void StatusBarNCurses::print_wait(const std::string &msg) {
  this->print(msg);
  this->get_input();
}

bool StatusBarNCurses::query_confirmation(const std::string &msg) {
  this->print(msg);
  auto ch = this->get_input();
  if (ch == 'y') {
    return true;
  } else {
    return false;
  }
}
