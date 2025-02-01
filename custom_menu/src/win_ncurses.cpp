#include "win_ncurses.h"
#include <ncurses.h>

namespace ncurses_lib {
  WinNCurses::WinNCurses()
    : win(init_window()) {
    keypad(win, true);
  }

  WinNCurses::~WinNCurses() { delwin(win); }

  int WinNCurses::get_input() {
    auto ch = wgetch(win);
    return ch;
  }

  void WinNCurses::refresh() const { wrefresh(win); }

  void WinNCurses::clear() const {
    werase(win);
    this->refresh();
  }

  WINDOW* WinNCurses::init_window() {
    int nx {};
    int ny {};
    getmaxyx(stdscr, nx, ny);
    int x {0};
    int y {0};
    return newwin(ny-1, nx-1, y, x);
  }
}
