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

  unsigned WinNCurses::n_lines() const { return getmaxx(win) + 1; }

  void WinNCurses::print_at(const std::string &str, int line) const {
    clear_line(line);
    mvwprintw(win, line, 0, "%s", str.c_str());
  }

  void WinNCurses::print_standout_at(const std::string &str, int line) const {
    wattron(win, A_STANDOUT);
    print_at(str, line);
    wattroff(win, A_STANDOUT);
  }

  void WinNCurses::clear_line(int line) const {
    wmove(win, line, 0);
    wclrtoeol(win);
  }

  WINDOW *WinNCurses::init_window() {
    int nx{};
    int ny{};
    getmaxyx(stdscr, nx, ny);
    int x{0};
    int y{0};
    return newwin(ny - 1, nx - 1, y, x);
  }
  } // namespace ncurses_lib
