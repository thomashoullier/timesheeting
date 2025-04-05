#ifndef MENU_NCURSES_H
#define MENU_NCURSES_H

#include "win_ncurses.h"
#include <string>
#include <vector>
#include <memory>

namespace ncurses_lib {
  struct MenuItem {
    /** @brief String displayed in the menu itself. Usually short. */
    std::string cell_string;
    /** @brief String for display on the status bar when queried. */
    std::string display_string;
    /** @brief Appearance of the cell string in the menu. */
    StringFace face;

    /** @brief MenuItem constructor. */
    MenuItem(const std::string &_cell_string,
             const std::string &_display_string,
             StringFace _face = StringFace::Normal);
  };

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
    /** @brief Empty string when nothing to return by reference. */
    std::string empty_string = " ";

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

  protected:
    /** @brief Get the current item index. */
    int get_item_index() const;
    /** @brief Get the current row index. */
    int get_row_index() const;
    /** @brief Get the current column index. */
    int get_col_index() const;

  public:
    /** @brief Return value for item selection:
               whether the selected item has actually changed or not. */
    enum class ItemSelectionStatus {same, changed};

    /** @brief Constructor.

        target_column_widths defines the number of desired width of columns.
        A width of >0 is fixed, the remaining window width is shared equally
        over the columns of width 0. */
    explicit MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items,
                         WindowPosition _winpos, WindowFormat _winformat,
                         std::vector<int> _target_column_widths);
    /**@brief Print all items to screen. */
    void refresh() override;
    /** @brief Select the item down of the current one. */
    ItemSelectionStatus select_down_item();
    /** @brief Select the item up of the current one. */
    ItemSelectionStatus select_up_item();
    /** @brief Select the item right of the current one. */
    ItemSelectionStatus select_right_item();
    /** @brief Select the item left of the current one. */
    ItemSelectionStatus select_left_item();
    /** @brief Get the full display string for the current item. */
    const std::string &get_current_item_string() const;
    /** @brief Set the currently held items. */
    void set_items(const std::shared_ptr<std::vector<MenuItem>> _items);
    /** @brief Resize event. */
    void resize() override;
  };

  /** @brief Exception when encountering an empty menu. */
  class MenuEmpty : public std::exception {
  private:
    /** @brief Exception message. */
    std::string msg;

  public:
    /** @brief Exception constructor. */
    explicit MenuEmpty (const char* _msg) : msg(_msg) {};
    /** @brief Mandatory std::exception method. */
    const char* what() const throw() { return msg.c_str(); };
  };
}


#endif // MENU_NCURSES_H
