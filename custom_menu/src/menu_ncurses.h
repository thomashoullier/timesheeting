#ifndef MENU_NCURSES_H
#define MENU_NCURSES_H

#include "win_ncurses.h"
#include <string>
#include <vector>
#include <memory>

namespace ncurses_lib {
  typedef std::string MenuItem;
  struct ColumnFormat {
    int pos; // Starting position in the window
    int width; // Width which can be used to write things.
  };

  class MenuNCurses : public WinNCurses {

  private:
    /** @brief Target column widths. */
    std::vector<int> target_column_widths;
    /** @brief Held items. */
    std::shared_ptr<std::vector<MenuItem>> items;
    /** @brief Currently selected item index. */
    int selected_index;
    /** @brief Scroll position. */
    int scroll_position;

    /** @brief Get the number of item columns. */
    int n_item_columns() const;
    /** @brief Distribute the target widths among columns. */
    std::vector<int> distribute_column_widths () const;
    /** @brief Compute the format of columns given the current window width. */
    std::vector<ColumnFormat> compute_columns() const;
    /** @brief Get the number of items currently held. */
    int n_items() const;
    /** @brief Get the cursor line position relative to the current window
    screen. */
    int cursor_line_position() const;
    /** @brief Get the cursor column position relative to the current window
               screen. */
    int cursor_col_position() const;
    /** @brief Get the width of the currently selected column. */
    int cursor_width() const;
    /** @brief Maximum allowable scroll_position value. */
    int max_scroll_position() const;
    /** @brief Scroll the whole screen down by one. */
    void scroll_down();
    /** @brief Scroll the whole screen up by one. */
    void scroll_up();

  public:
    /** @brief Constructor.

     target_column_widths defines the number of desired width of columns.
     A width of >0 is fixed, the remaining window width is shared equally
     over the columns of width 0. */
    explicit MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items,
                         int _top_pos, int _bottom_pos,
                         WindowHorizontal _horizontal_layout,
                         std::vector<int> _target_column_widths);

    /**@brief Print all items to screen. */
    void print_items() const;
    /** @brief Select the item down of the current one. */
    void select_down_item();
    /** @brief Select the item up of the current one. */
    void select_up_item();
    /** @brief Select the item right of the current one. */
    void select_right_item();
    /** @brief Select the item left of the current one. */
    void select_left_item();
    /** @brief Resize event. */
    void resize();
  };
}


#endif // MENU_NCURSES_H
