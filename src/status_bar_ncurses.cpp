#include "status_bar_ncurses.h"
#include <curses.h>

StatusBarNCurses::StatusBarNCurses()
  : win(init_window()) {}

WINDOW *StatusBarNCurses::init_window() {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);
  return newwin(1, max_x, max_y - 1, 0);
}

StatusBarNCurses::~StatusBarNCurses() {
  delwin(win);
}
void StatusBarNCurses::print(const std::string &msg) {
  wclear(win);
  wprintw(win, msg.c_str());
  wrefresh(win);
}

bool StatusBarNCurses::query_confirmation(const std::string &msg) {
  print(msg);
  auto ch = wgetch(win);
  if (ch == 'y') {
    return true;
  } else {
    return false;
  }
}
