#include "win_ncurses.h"
#include <ncurses.h>

namespace ncurses_lib {
  WinNCurses::WinNCurses(unsigned _top_pos, unsigned _bottom_pos)
    : top_position{_top_pos}, bottom_position{_bottom_pos}, win(init_window()) {
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

  unsigned WinNCurses::n_lines() const { return getmaxy(win); }

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
    getmaxyx(stdscr, ny, nx);
    int x{0};
    int target_lines = ny - bottom_position - top_position;
    return newwin(target_lines, nx, top_position,
                  x);
  }

  void WinNCurses::resize() {
    int nx{};
    int ny{};
    getmaxyx(stdscr, ny, nx);
    int target_lines = ny - bottom_position - top_position;
    if (target_lines >= 1) {
      delwin(win);
      win = newwin(target_lines, nx, top_position, 0);
    }
  }

  } // namespace ncurses_lib
