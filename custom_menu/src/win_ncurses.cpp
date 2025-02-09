#include "win_ncurses.h"
#include <ncurses.h>

namespace ncurses_lib {
WinNCurses::WinNCurses(int _top_pos, int _bottom_pos,
                       WindowHorizontal _horizontal_layout)
    : top_position{_top_pos}, bottom_position{_bottom_pos},
      horizontal_layout{_horizontal_layout}, border_on{false},
      win(init_window()) {
  keypad(win, true);
  draw_border();
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

int WinNCurses::n_lines() const { return getmaxy(win) - 1; }

int WinNCurses::n_cols() const { return getmaxx(win); }

void WinNCurses::print_at(const std::string &str, int line, int col_offset,
                          int col_width) const {
  const std::string eraser(col_width, ' ');
  // We offset the printing by one since the top line is reserved for
  // a border
  mvwprintw(win, line + 1, col_offset, "%s", eraser.c_str());
  mvwprintw(win, line + 1, col_offset, "%s",
            str.substr(0, n_cols() - col_offset).c_str());
  }

  void WinNCurses::print_standout_at(const std::string &str,
                                     int line, int col_offset,
                                     int col_width) const {
    wattron(win, A_STANDOUT);
    print_at(str, line, col_offset, col_width);
    wattroff(win, A_STANDOUT);
  }

  WINDOW *WinNCurses::init_window() {
    int nx{};
    int ny{};
    getmaxyx(stdscr, ny, nx);
    // Computing the position and size in the horizontal direction
    int x{};
    switch (horizontal_layout) {
    case WindowHorizontal::full:
      x = 0;
      break;
    case WindowHorizontal::right:
      nx = nx / 2;
      x = nx;
      break;
    case WindowHorizontal::left:
      if (nx % 2 == 0)
        nx = nx / 2;
      else
        nx = nx / 2 + 1;
      x = 0;
      break;
    }
    int target_lines = ny - bottom_position - top_position;
    return newwin(target_lines, nx, top_position, x);
  }

  void WinNCurses::resize() {
    int nx{};
    int ny{};
    getmaxyx(stdscr, ny, nx);
    int target_lines = ny - bottom_position - top_position;
    if (target_lines >= 1) {
      delwin(win);
      win = init_window();
      draw_border();
    }
  }

  void WinNCurses::draw_border() const {
    if (border_on) {
      wmove(win, 0, 0);
      wclrtoeol(win);
      const std::string borderstr(n_cols(), '-');
      mvwprintw(win, 0, 0, "%s", borderstr.c_str());
    }
  }

  void WinNCurses::set_border() {
    border_on = true;
  }

  void WinNCurses::unset_border() {
    border_on = false;
  }

} // namespace ncurses_lib
