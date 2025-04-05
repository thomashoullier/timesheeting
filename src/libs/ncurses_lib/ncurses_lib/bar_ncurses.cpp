#include "bar_ncurses.h"
#include "win_ncurses.h"
#include <ncurses.h>
#include <stdexcept>

namespace ncurses_lib {
  BarNCurses::BarNCurses(WindowPosition winpos, WindowFormat winformat)
    : WinNCurses(winpos, filter_window_format(winformat)) {}

  void BarNCurses::print(const std::string &msg) {
    clear();
    wmove(win, 0, 0);
    wprintw(win, "%s", msg.c_str());
    BarNCurses::refresh();
  }

  void BarNCurses::print_right(const std::string &msg) {
    clear();
    int str_len = msg.length();
    int max_x = getmaxx(win);
    wmove(win, 0, max_x - str_len);
    wprintw(win, "%s", msg.c_str());
    BarNCurses::refresh();
  }

  void BarNCurses::print_after_cursor(const std::string &msg) {
    int y, x;
    getyx(win, y, x);
    wclrtoeol(win); // Clear after the current content.
    if (x + 2 + msg.length() < max_size()) { // Print only if there is room.
      wattron(win, A_STANDOUT);
      wmove(win, y, x + 2);
      wprintw(win, "%s", msg.c_str());
      wattroff(win, A_STANDOUT);
      wmove(win, y, x); // Reset cursor position.
    }
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

  std::size_t BarNCurses::max_size() const { return (std::size_t)getmaxx(win); }

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
}
