#include "bar_ncurses.h"
#include "win_ncurses.h"

BarNCurses::BarNCurses(WindowPosition winpos)
  : WinNCurses(winpos, WindowFormat::line) {}

void BarNCurses::print(const std::string &msg) const {
  this->clear();
  wmove(win, 0, 0);
  wprintw(win, msg.c_str());
  this->refresh();
}
