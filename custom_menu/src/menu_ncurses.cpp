#include "menu_ncurses.h"
#include "win_ncurses.h"
#include <algorithm>
#include <numeric>
#include <curses.h>
#include <iostream>
#include <ncurses.h>

namespace ncurses_lib {
  MenuItem::MenuItem(const std::string &_cell_string,
                     const std::string &_display_string,
                     StringFace _face)
    : cell_string{_cell_string}, display_string{_display_string}, face{_face} {}


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

  std::vector<int> MenuNCurses::distribute_column_widths() const {
    int total_fixed_width = std::accumulate(target_column_widths.begin(),
                                            target_column_widths.end(),
                                            0);
    // Available target writable width for columns set to zero.
    // We count one unit of gap to the right of each column.
    int available_width = n_cols() - total_fixed_width - n_item_columns();
    // Number of columns with variable width
    int n_var_cols = std::count(target_column_widths.begin(),
                                target_column_widths.end(),
                                0);
    std::vector<int> widths;
    for (const auto &w : target_column_widths) {
      if (w != 0) { // Fixed width column.
        widths.push_back(w);
      } else { // Variable width column.
        // Base size for columns.
        int base_size = available_width / n_var_cols;
        // Number of columns which need to be assigned +1.
        int n_larger_cols = available_width % n_var_cols;
        int var_w = base_size;
        if (n_larger_cols > 0) {
          var_w += 1;
          --n_larger_cols;
        }
        widths.push_back(var_w);
      }
    }
    return widths;
  }

  std::vector<ColumnFormat> MenuNCurses::compute_columns() const {
    std::vector<ColumnFormat> cols;
    int cur_pos{0};
    int next_pos{0};
    auto distributed_widths = distribute_column_widths();
    for (const auto &target_width : distributed_widths) {
      next_pos += target_width + 1; // Gap of one on the right of columns.
      cols.push_back({std::min(cur_pos, n_cols() - 1),
                      std::max(0, next_pos - cur_pos - 1)});
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

  void MenuNCurses::refresh() const {
    WinNCurses::refresh();
    auto i_item = n_item_columns() * scroll_position;
    auto start_item = i_item;
    int icol = (i_item - start_item) % n_item_columns();
    auto cols = compute_columns();
    int iline = (i_item - start_item) / n_item_columns();
    while (i_item < n_items() && iline < n_lines()) {
      auto col = cols.at(icol);
      auto item_str = items->at(i_item).cell_string;
      if (i_item == selected_index) {
        print_standout_at(item_str.substr(0, col.width), iline,
                          col.pos, col.width, items->at(i_item).face);
      } else {
        print_at(item_str.substr(0, col.width), iline, col.pos,
                 col.width, items->at(i_item).face);
      }
      ++i_item;
      icol = (i_item - start_item) % n_item_columns();
      iline = (i_item - start_item) / n_item_columns();
    }
  }

  void MenuNCurses::scroll_down() {
    ++scroll_position;
    refresh();
  }

  void MenuNCurses::scroll_up() {
    --scroll_position;
    refresh();
  }

  void MenuNCurses::select_down_item() {
    if (selected_index + n_item_columns() < n_items()) {
      if (cursor_line_position() == n_lines() - 1) {
        selected_index += n_item_columns();
        scroll_down();
      } else {
        // Reprint the current item as non-highlighted
        print_at(items->at(selected_index).cell_string,
                 cursor_line_position(),
                 cursor_col_position(), cursor_width(),
                 items->at(selected_index).face);
        // Reprint the selected item as highlighted.
        selected_index += n_item_columns();
        print_standout_at(items->at(selected_index).cell_string,
                          cursor_line_position(), cursor_col_position(),
                          cursor_width(),
                          items->at(selected_index).face);
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
        print_at(items->at(selected_index).cell_string,
                 cursor_line_position(),
                 cursor_col_position(), cursor_width(),
                 items->at(selected_index).face);
        // Reprint the selected item as highlighted.
        selected_index -= n_item_columns();
        print_standout_at(items->at(selected_index).cell_string,
                          cursor_line_position(), cursor_col_position(),
                          cursor_width(),
                          items->at(selected_index).face);
      }
    }
  }

  void MenuNCurses::select_right_item() {
    if (selected_index % n_item_columns() + 1 < n_item_columns()) {
      // Reprint the current item as non-highlighted
      print_at(items->at(selected_index).cell_string,
               cursor_line_position(),
               cursor_col_position(), cursor_width(),
               items->at(selected_index).face);
      ++selected_index;
      // Reprint the selected item as highlighted.
      print_standout_at(items->at(selected_index).cell_string,
                        cursor_line_position(), cursor_col_position(),
                        cursor_width(), items->at(selected_index).face);
    }
  }

  void MenuNCurses::select_left_item() {
    if (selected_index % n_item_columns() - 1 >= 0) {
      // Reprint the current item as non-highlighted
      print_at(items->at(selected_index).cell_string,
               cursor_line_position(),
               cursor_col_position(), cursor_width(),
               items->at(selected_index).face);
      --selected_index;
      // Reprint the selected item as highlighted.
      print_standout_at(items->at(selected_index).cell_string,
                        cursor_line_position(), cursor_col_position(),
                        cursor_width(),
                        items->at(selected_index).face);
    }
  }

  void MenuNCurses::set_items
  (const std::shared_ptr<std::vector<MenuItem>>  _items) {
    items = _items;
    refresh();
  }

  void MenuNCurses::resize() {
    clear();
    WinNCurses::resize();
    if (cursor_line_position() >= n_lines())
      scroll_position = selected_index / n_item_columns();
    if (scroll_position > max_scroll_position())
      scroll_position = max_scroll_position();
    refresh();
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
