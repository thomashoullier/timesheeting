#include "win_ncurses.h"

WinNCurses::WinNCurses() : win(init_window()) {}

WinNCurses::~WinNCurses() { delwin(win); }

char WinNCurses::get_input() { return wgetch(win); }

void WinNCurses::refresh() { wrefresh(win); }

void WinNCurses::clear() { werase(win); }

WINDOW* WinNCurses::init_window() {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);
  return newwin(1, max_x, max_y - 1, 0);
}
