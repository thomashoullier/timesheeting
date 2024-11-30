#include "ncurses_handle.h"

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
