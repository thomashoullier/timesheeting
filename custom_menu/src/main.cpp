#include "menu_ncurses.h"
#include "ncurses_handle.h"
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <vector>

int main () {
  auto ncurses_handle = ncurses_lib::NcursesHandle();

  int n_items = 10;
  auto items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
  for (int i = 0 ; i < n_items ; ++i) {
    items->push_back("Item" + std::to_string(i));
  }

  auto menu = ncurses_lib::MenuNCurses(items, 2, 4);
  menu.print_items();

  // Input loop
  while (true) {
    auto ch = menu.get_input();
    switch (ch) {
    case 'q':
      return 0; // Quit
    case 'n': // Down
      menu.select_down_item();
      break;
    case 'e': // Up
      menu.select_up_item();
      break;
    case KEY_RESIZE:
      std::cerr << "Resize caught." << std::endl;
      menu.resize();
      break;
    }
  }
}
