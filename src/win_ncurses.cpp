#include "win_ncurses.h"
#include <stdexcept>

WinNCurses::WinNCurses(WindowPosition winpos, WindowFormat winformat)
  : win(init_window(winpos, winformat)) {}

WinNCurses::~WinNCurses() { delwin(win); }

char WinNCurses::get_input() { return wgetch(win); }

void WinNCurses::refresh() const { wrefresh(win); }

void WinNCurses::clear() const {
  werase(win);
  this->refresh();
}

WINDOW* WinNCurses::init_window(WindowPosition winpos, WindowFormat winformat) {
  auto max_y = getmaxy(stdscr);
  int WIDTH {80};
  int y;
  switch (winpos) {
  case WindowPosition::top:
    y = 1;
    break;
  case WindowPosition::bottom:
    y = max_y - 1;
    break;
  case WindowPosition::upper:
    y = 2;
    break;
  default:
    throw std::logic_error("WinNCurses: unknown WindowPosition.");
  }
  int ny;
  switch(winformat) {
  case WindowFormat::line:
    ny = 1;
    break;
  case WindowFormat::block:
    ny = 35;
    break;
  default:
    throw std::logic_error("WinNCurses: unknown WindowFormat.");
  }
  return newwin(ny, WIDTH, y, 0);
}
