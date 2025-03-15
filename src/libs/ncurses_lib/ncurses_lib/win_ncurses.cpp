#include "win_ncurses.h"
#include <iostream>
#include <ncurses.h>
#include <stdexcept>

namespace ncurses_lib {
  WinNCurses::WinNCurses(WindowPosition _winpos, WindowFormat _winformat)
    : winpos{_winpos}, winformat{_winformat}, border_on{false},
      win(init_window()) {
  keypad(win, true);
  draw_border();
}

WinNCurses::~WinNCurses() { delwin(win); }

int WinNCurses::get_input() {
  auto ch = wgetch(win);
  return ch;
}

void WinNCurses::refresh() const {
  if (border_on)
    draw_border();
  wrefresh(win);
}

void WinNCurses::clear() const {
  werase(win);
  this->refresh();
}

int WinNCurses::n_lines() const { return getmaxy(win) - 1; }

int WinNCurses::n_cols() const { return getmaxx(win); }

void WinNCurses::print_at(const std::string &str, int line, int col_offset,
                          int col_width, StringFace face) const {
  switch (face) {
  case StringFace::Normal:
    break;
  case StringFace::Bold:
    wattron(win, A_BOLD);
    break;
  case StringFace::Italic:
    wattron(win, A_ITALIC);
    break;
  default:
    throw std::runtime_error("WinNCurses::print_at: unknown face");
  }
  const std::string eraser(col_width, ' ');
  // We offset the printing by one since the top line is reserved for
  // a border
  mvwprintw(win, line + 1, col_offset, "%s", eraser.c_str());
  mvwprintw(win, line + 1, col_offset, "%s",
            str.substr(0, col_width).c_str());
  switch (face) {
  case StringFace::Normal:
    break;
  case StringFace::Bold:
    wattroff(win, A_BOLD);
    break;
  case StringFace::Italic:
    wattroff(win, A_ITALIC);
    break;
  default:
    throw std::runtime_error("WinNCurses::print_at: unknown face");
  }
}

  void WinNCurses::print_standout_at(const std::string &str,
                                     int line, int col_offset,
                                     int col_width,
                                     StringFace face) const {
    wattron(win, A_STANDOUT);
    print_at(str, line, col_offset, col_width, face);
    wattroff(win, A_STANDOUT);
  }


  std::array<int, 4> WinNCurses::compute_window_dimensions () const {
    // Available standard screen dimensions.
    int max_x{};
    int max_y{};
    getmaxyx(stdscr, max_y, max_x);
    // Compute the window position
    int y, x;
    switch (winpos) {
    case WindowPosition::top:
      x = 0;
      y = 1;
      break;
    case WindowPosition::top_left:
      x = 0;
      y = 1;
      break;
    case WindowPosition::top_right:
      x = max_x / 2;
      y = 1;
      break;
    case WindowPosition::bottom:
      x = 0;
      y = max_y - 1;
      break;
    case WindowPosition::upper:
      x = 0;
      y = 2;
      break;
    case WindowPosition::lower:
      x = 0;
      y = max_y - 4;
      break;
    case WindowPosition::left:
      x = 0;
      y = 2;
      break;
    case WindowPosition::middle:
      x = max_x / 2;
      y = 2;
      break;
    default:
      throw std::logic_error("WinNCurses: unknown WindowPosition.");
    }
    int ny, nx;
    switch(winformat) {
    case WindowFormat::line:
      nx = max_x;
      ny = 1;
      break;
    case WindowFormat::box:
      nx = max_x;
      ny = 2;
      break;
    case WindowFormat::half_line:
      nx = max_x / 2;
      ny = 1;
      break;
    case WindowFormat::block:
      nx = max_x;
      ny = max_y - 6;
      break;
    case WindowFormat::column:
      nx = max_x / 2;
      // Fix overlap for the left part
      if (max_x % 2 != 0 && x == 0) {
        nx = max_x / 2 + 1;
      }
      ny = max_y - 6;
      break;
    default:
      throw std::logic_error("WinNCurses: unknown WindowFormat.");
    }
    return {ny, nx, y, x};
  }

  WINDOW *WinNCurses::init_window() {
    auto dims = compute_window_dimensions();
    return newwin(dims.at(0), dims.at(1), dims.at(2), dims.at(3));
  }

  void WinNCurses::resize() {
    auto dims = compute_window_dimensions();
    auto ny = dims.at(0);
    auto nx = dims.at(1);
    if (ny >= 1 && nx >= 1) {
      delwin(win);
      win = init_window();
      draw_border();
    }
  }

  void WinNCurses::draw_border() const {
    wmove(win, 0, 0);
    wclrtoeol(win);
    if (border_on) {
      const std::string borderstr(n_cols(), '-');
      mvwprintw(win, 0, 0, "%s", borderstr.c_str());
    }
  }

  void WinNCurses::set_border() {
    border_on = true;
    draw_border();
    refresh();
  }

  void WinNCurses::unset_border() {
    border_on = false;
    draw_border();
    refresh();
  }

} // namespace ncurses_lib
