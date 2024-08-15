#include "ncurses_handle.h"

NcursesHandle::NcursesHandle() {
  initscr();
  cbreak();
  noecho();
}

NcursesHandle::~NcursesHandle() {
  endwin();
}
