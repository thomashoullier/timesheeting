#include "ncurses_handle.h"

NcursesHandle::NcursesHandle() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
}

NcursesHandle::~NcursesHandle() {
  endwin();
}
