#include "bar_ncurses.h"
#include "win_ncurses.h"
#include <stdexcept>

BarNCurses::BarNCurses(WindowPosition winpos, WindowFormat winformat)
    : WinNCurses(winpos, filter_window_format(winformat)) {}

void BarNCurses::print(const std::string &msg) const {
  this->clear();
  wmove(win, 0, 0);
  wprintw(win, msg.c_str());
  this->refresh();
}

void BarNCurses::print_right(const std::string &msg) const {
  this->clear();
  int str_len = msg.length();
  int max_x = getmaxx(win);
  wmove(win, 0, max_x - str_len);
  wprintw(win, msg.c_str());
  this->refresh();
}

void BarNCurses::set_cursor_visibility(bool visible) {
  if (visible)
    curs_set(1);
  else
    curs_set(0);
}

void BarNCurses::prepare_input() {
  wmove(win, 0, 0);
  wclrtoeol(win);
}

void BarNCurses::add_char(char ch) { waddch(win, ch); }

void BarNCurses::remove_char() {
  int y, x;
  getyx(win, y, x);
  wmove(win, y, x - 1);
  wdelch(win);
}

std::size_t BarNCurses::max_size() const {
  return (std::size_t) getmaxx(win);
}

WindowFormat BarNCurses::filter_window_format(WindowFormat winformat) {
  switch (winformat) {
  case WindowFormat::line:
    return winformat;
  case WindowFormat::half_line:
    return winformat;
  default:
    throw std::logic_error("WindowFormat is incompatible with BarNCurses.");
  }
}
