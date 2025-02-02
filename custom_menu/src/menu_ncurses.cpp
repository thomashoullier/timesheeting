#include "menu_ncurses.h"
#include "win_ncurses.h"
#include <curses.h>
#include <iostream>
#include <ncurses.h>

namespace ncurses_lib {
  MenuNCurses::MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items,
                           int _top_pos, int _bottom_pos)
    : WinNCurses(_top_pos, _bottom_pos),
      items{_items}, selected_index{0}, scroll_position{0} {}

  int MenuNCurses::n_items() const {
    return items->size();
  }

  int MenuNCurses::cursor_position() const {
    return selected_index - scroll_position;
  }

  int MenuNCurses::max_scroll_position() const {
    int val = n_items() - n_lines();
    if (val <= 0)
      return 0;
    else
      return val;
  }

  void MenuNCurses::print_items() const {
    auto i_item = scroll_position;
    for (int i_line = 0 ;
         i_line < n_lines() && i_item < n_items() ;
         ++i_line) {
      if (i_item == selected_index)
        print_standout_at(items->at(i_item), i_line);
      else
        print_at(items->at(i_item), i_line);
      ++i_item;
    }
  }

  void MenuNCurses::scroll_down() {
    ++scroll_position;
    print_items();
  }

  void MenuNCurses::scroll_up() {
    --scroll_position;
    print_items();
  }

  void MenuNCurses::select_down_item() {
    if (selected_index < n_items() - 1) {
      if (cursor_position() == n_lines() - 1) {
        ++selected_index;
        scroll_down();
      } else {
        // Reprint the current item as non-highlighted
        print_at(items->at(selected_index), cursor_position());
        // Reprint the selected item as highlighted.
        ++selected_index;
        print_standout_at(items->at(selected_index), cursor_position());
      }
    }
  }

  void MenuNCurses::select_up_item() {
    if (selected_index > 0) {
      if (cursor_position() == 0) {
        --selected_index;
        scroll_up();
      } else {
        // Reprint the current item as non-highlighted
        print_at(items->at(selected_index), cursor_position());
        // Reprint the selected item as highlighted.
        --selected_index;
        print_standout_at(items->at(selected_index), cursor_position());
      }
    }
  }

  void MenuNCurses::resize() {
    clear();
    WinNCurses::resize();
    if (cursor_position() >= n_lines())
      scroll_position = selected_index;
    if (scroll_position > max_scroll_position())
      scroll_position = max_scroll_position();
    print_items();
    std::cerr << "After resize: "
              << "selected_index: " << selected_index << ", "
              << "window n_lines: " << n_lines() << ", "
              << "cursor_position: " << cursor_position() << ", "
              << "scroll_position: " << scroll_position << ", "
              << "max_scroll_position: " << max_scroll_position() << ", "
              << std::endl;
  }
}
