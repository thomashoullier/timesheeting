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

    /** @brief Get the number of items currently held. */
    unsigned n_items() const;

  public:
    explicit MenuNCurses(const std::shared_ptr<std::vector<MenuItem>> _items);

    /**@brief Print all items to screen. */
    void print_items() const;
    /** @brief Select the item down of the current one. */
    void select_down_item();
    /** @brief Select the item up of the current one. */
    void select_up_item();
  };
}


#endif // MENU_NCURSES_H
