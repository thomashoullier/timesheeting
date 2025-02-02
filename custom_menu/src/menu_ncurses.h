#ifndef MENU_NCURSES_H
#define MENU_NCURSES_H

#include "win_ncurses.h"
#include <string>
#include <vector>
#include <memory>

namespace ncurses_lib {
  typedef std::string MenuItem;

  class MenuNCurses : public WinNCurses {

  private:
    std::shared_ptr<std::vector<MenuItem>> items;
    /** @brief Currently selected item index. */
    int selected_index;
    /** @brief Scroll position. */
    int scroll_position;

    /** @brief Get the number of items currently held. */
    int n_items() const;
    /** @brief Get the cursor position relative to the current window screen. */
    int cursor_position() const;
    /** @brief Maximum allowable scroll_position value. */
    int max_scroll_position() const;
    /** @brief Scroll the whole screen down by one. */
    void scroll_down();
    /** @brief Scroll the whole screen up by one. */
    void scroll_up();

  public:
    explicit MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items,
                         int _top_pos, int _bottom_pos);

    /**@brief Print all items to screen. */
    void print_items() const;
    /** @brief Select the item down of the current one. */
    void select_down_item();
    /** @brief Select the item up of the current one. */
    void select_up_item();
    /** @brief Resize event. */
    void resize();
  };
}


#endif // MENU_NCURSES_H
