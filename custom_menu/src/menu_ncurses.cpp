#include "menu_ncurses.h"
#include "win_ncurses.h"
#include <algorithm>
#include <curses.h>
#include <iostream>
#include <ncurses.h>

namespace ncurses_lib {
MenuNCurses::MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items,
                         int _top_pos, int _bottom_pos,
                         WindowHorizontal _horizontal_layout,
                         std::vector<int> _target_column_widths)
    : WinNCurses(_top_pos, _bottom_pos, _horizontal_layout),
      target_column_widths{_target_column_widths}, items{_items},
      selected_index{0}, scroll_position{0} {}

  int MenuNCurses::n_item_columns() const {
    return target_column_widths.size();
  }

std::vector<ColumnFormat> MenuNCurses::compute_columns() const {
  // TODO: manage the 0 case for widths.
  std::vector<ColumnFormat> cols;
  int cur_pos{0};
  int next_pos{0};
  for (const auto &target_width : target_column_widths) {
    next_pos += target_width + 1; // Gap of one on the right of columns.
    cols.push_back({cur_pos, next_pos - cur_pos - 1});
    cur_pos = next_pos;
  }
  return cols;
}

  int MenuNCurses::n_items() const {
    return items->size();
  }

  int MenuNCurses::cursor_line_position() const {
    return selected_index / n_item_columns() - scroll_position;
  }

  int MenuNCurses::cursor_col_position() const {
    auto cols = compute_columns();
    int col_index = selected_index % n_item_columns();
    return cols.at(col_index).pos;
  }

  int MenuNCurses::cursor_width() const {
    auto cols = compute_columns();
    int col_index = selected_index % n_item_columns();
    return cols.at(col_index).width;
  }

  int MenuNCurses::max_scroll_position() const {
    int val = n_items() / n_item_columns() - n_lines();
    return std::max(0, val);
  }

  void MenuNCurses::print_items() const {
    auto i_item = n_item_columns() * scroll_position;
    auto start_item = i_item;
    int icol = (i_item - start_item) % n_item_columns();
    auto cols = compute_columns();
    int iline = (i_item - start_item) / n_item_columns();
    while (i_item < n_items() && iline < n_lines()) {
      auto col = cols.at(icol);
      if (i_item == selected_index) {
        print_standout_at(items->at(i_item).substr(0, col.width), iline,
                          col.pos, col.width);
      } else {
        print_at(items->at(i_item).substr(0, col.width), iline, col.pos,
                 col.width);
      }
      ++i_item;
      icol = (i_item - start_item) % n_item_columns();
      iline = (i_item - start_item) / n_item_columns();
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
    if (selected_index + n_item_columns() < n_items()) {
      if (cursor_line_position() == n_lines() - 1) {
        selected_index += n_item_columns();
        scroll_down();
      } else {
        // Reprint the current item as non-highlighted
        print_at(items->at(selected_index), cursor_line_position(),
                 cursor_col_position(), cursor_width());
        // Reprint the selected item as highlighted.
        selected_index += n_item_columns();
        print_standout_at(items->at(selected_index), cursor_line_position(),
                          cursor_col_position(), cursor_width());
      }
    }
  }

  void MenuNCurses::select_up_item() {
    if (selected_index - n_item_columns() >= 0) {
      if (cursor_line_position() == 0) {
        selected_index -= n_item_columns();
        scroll_up();
      } else {
        // Reprint the current item as non-highlighted
        print_at(items->at(selected_index), cursor_line_position(),
                 cursor_col_position(), cursor_width());
        // Reprint the selected item as highlighted.
        selected_index -= n_item_columns();
        print_standout_at(items->at(selected_index), cursor_line_position(),
                          cursor_col_position(), cursor_width());
      }
    }
  }

  void MenuNCurses::select_right_item() {
    if (selected_index % n_item_columns() + 1 < n_item_columns()) {
      // Reprint the current item as non-highlighted
      print_at(items->at(selected_index), cursor_line_position(),
               cursor_col_position(), cursor_width());
      ++selected_index;
      // Reprint the selected item as highlighted.
      print_standout_at(items->at(selected_index), cursor_line_position(),
                        cursor_col_position(), cursor_width());
    }
  }

  void MenuNCurses::select_left_item() {
    if (selected_index % n_item_columns() - 1 >= 0) {
      // Reprint the current item as non-highlighted
      print_at(items->at(selected_index), cursor_line_position(),
               cursor_col_position(), cursor_width());
      --selected_index;
      // Reprint the selected item as highlighted.
      print_standout_at(items->at(selected_index), cursor_line_position(),
                        cursor_col_position(), cursor_width());
    }
  }

  void MenuNCurses::resize() {
    clear();
    WinNCurses::resize();
    if (cursor_line_position() >= n_lines())
      scroll_position = selected_index / n_item_columns();
    if (scroll_position > max_scroll_position())
      scroll_position = max_scroll_position();
    print_items();
    std::cerr << "After resize: "
              << "selected_index: " << selected_index << ", "
              << "window n_lines: " << n_lines() << ", "
              << "window n_cols: " << n_cols() << ", "
              << "cursor_position: " << cursor_line_position() << ", "
              << "scroll_position: " << scroll_position << ", "
              << "max_scroll_position: " << max_scroll_position() << ", "
              << std::endl;
  }
} // namespace ncurses_lib
