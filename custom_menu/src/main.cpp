#include "menu_ncurses.h"
#include "ncurses_handle.h"
#include "win_ncurses.h"
#include <memory>
#include <string>
#include <vector>

int main () {
  auto ncurses_handle = ncurses_lib::NcursesHandle();

  auto menu = ncurses_lib::MenuNCurses();

  menu.print_items();

  menu.get_input();
  //items->at(1) = "UpdatedItem2";
  menu.print_items();
  menu.get_input();
}
