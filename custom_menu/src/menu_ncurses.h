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
    unsigned selected_index;
    /** @brief Scroll position. */
    unsigned scroll_position;

    /** @brief Get the number of items currently held. */
    unsigned n_items() const;
    /** @brief Get the cursor position relative to the current window screen. */
    unsigned cursor_position() const;
    /** @brief Maximum allowable scroll_position value. */
    unsigned max_scroll_position() const;
    /** @brief Scroll the whole screen down by one. */
    void scroll_down();
    /** @brief Scroll the whole screen up by one. */
    void scroll_up();

  public:
    explicit MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items,
                         unsigned _top_pos, unsigned _bottom_pos);

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
