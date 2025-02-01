#include "menu_ncurses.h"
#include <curses.h>

namespace ncurses_lib {
  MenuNCurses::MenuNCurses() {

  }

  void MenuNCurses::print_items() {
    mvwprintw(win, 0, 0, "Hello");
    mvwprintw(win, 1, 0, "There");
  }
}
