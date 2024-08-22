#include "win_ncurses.h"
#include <stdexcept>

WinNCurses::WinNCurses(WindowPosition winpos) : win(init_window(winpos)) {}

WinNCurses::~WinNCurses() { delwin(win); }

char WinNCurses::get_input() { return wgetch(win); }

void WinNCurses::refresh() const { wrefresh(win); }

void WinNCurses::clear() const {
  werase(win);
  this->refresh();
}

WINDOW* WinNCurses::init_window(WindowPosition winpos) {
  int max_y, max_x, y;
  getmaxyx(stdscr, max_y, max_x);
  switch (winpos) {
  case WindowPosition::top:
    y = 1;
    break;
  case WindowPosition::bottom:
    y = max_y - 1;
    break;
  default:
    throw std::logic_error("WinNCurses: unknown WindowPosition.");
  }
  return newwin(1, max_x, y, 0);
}
