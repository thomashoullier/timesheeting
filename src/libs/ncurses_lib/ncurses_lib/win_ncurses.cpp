#include "win_ncurses.h"
#include "log_lib/logger.h"
#include <stdexcept>

namespace ncurses_lib {
  WinNCurses::WinNCurses(WindowPosition winpos, WindowFormat winformat)
    : win(init_window(winpos, winformat)) {
    keypad(win, true);
  }

  WinNCurses::~WinNCurses() { destroy_window(); }

  int WinNCurses::get_input() {
    // TODO: decouple from the logger.
    log_lib::logger().tock();
    auto ch = wgetch(win);
    log_lib::logger().tick();
    return ch;
  }

  void WinNCurses::refresh() const { wrefresh(win); }

  void WinNCurses::clear() const {
    werase(win);
    this->refresh();
  }

  void WinNCurses::destroy_window() { delwin(win); }

  WINDOW* WinNCurses::init_window(WindowPosition winpos,
                                  WindowFormat winformat) {
    auto max_y = getmaxy(stdscr);
    int WIDTH {80};
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
      x = WIDTH / 2;
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
      x = WIDTH / 2;
      y = 2;
      break;
    default:
      throw std::logic_error("WinNCurses: unknown WindowPosition.");
    }
    int ny, nx;
    switch(winformat) {
    case WindowFormat::line:
      nx = WIDTH;
      ny = 1;
      break;
    case WindowFormat::box:
      nx = WIDTH;
      ny = 2;
      break;
    case WindowFormat::half_line:
      nx = WIDTH / 2;
      ny = 1;
      break;
    case WindowFormat::block:
      nx = WIDTH;
      ny = 35;
      break;
    case WindowFormat::column:
      nx = WIDTH / 2;
      ny = 35;
      break;
    default:
      throw std::logic_error("WinNCurses: unknown WindowFormat.");
    }
    return newwin(ny, nx, y, x);
  }
}
