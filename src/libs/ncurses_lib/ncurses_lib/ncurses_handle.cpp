#include "ncurses_handle.h"
#include <curses.h>

namespace ncurses_lib {
  NcursesHandle::NcursesHandle() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
  }

  NcursesHandle::~NcursesHandle() {
    endwin();
  }
}
