#include "menu_ncurses.h"
#include <curses.h>
#include <ncurses.h>

namespace ncurses_lib {
  MenuNCurses::MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items)
    : items{_items}, selected_index{0} {}

  unsigned MenuNCurses::n_items() const {
    return items->size();
  }

  void MenuNCurses::print_items() const {
    unsigned i {0};
    for (const auto &it : *items) {
      if (i == selected_index)
        print_standout_at(it, i);
      else
        print_at(it, i);
      ++i;
    }
  }

  void MenuNCurses::select_down_item() {
    if (selected_index < n_items() - 1) {
      // Reprint the current item as non-highlighted
      print_at(items->at(selected_index), selected_index);
      // Reprint the selected item as highlighted.
      ++selected_index;
      print_standout_at(items->at(selected_index), selected_index);
    }
  }

  void MenuNCurses::select_up_item() {
    if (selected_index > 0) {
      // Reprint the current item as non-highlighted
      print_at(items->at(selected_index), selected_index);
      // Reprint the selected item as highlighted.
      --selected_index;
      print_standout_at(items->at(selected_index), selected_index);
    }
  }
}
