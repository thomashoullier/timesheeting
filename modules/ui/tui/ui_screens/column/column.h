/** @file
 * @brief Column definition. */
#ifndef COLUMN_H
#define COLUMN_H

#include "column_base.h"
#include "core/generic_item.h"
#include "ncurses_lib/menu_ncurses.h"
#include <type_traits>

namespace tui {
  /** @brief Column menu class. Can hold any GenericItem. */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<core::GenericItem,
                                                          T>>>
  class Column : public ColumnBase, public ncurses_lib::MenuNCurses {
  public:
    /** @brief Construct the column with given items and position. */
    Column(const std::vector<T> &items, ncurses_lib::WindowPosition winpos)
      : MenuNCurses(items_to_string(items), winpos,
                    ncurses_lib::WindowFormat::column, 1),
        held_items(items) {};

    /** @brief Replace the column items. */
    void set_items(const std::vector<T> &items) {
      held_items = items;
      MenuNCurses::set_items(items_to_string(items));
    };

    core::Id get_current_id() override {
      if (held_items.empty()) {
        throw(ncurses_lib::MenuEmpty
              ("get_current_id(): no items in the register!"));
      }
      auto item_index = get_row_index();
      return held_items.at(item_index).id;
    };

    std::string get_current_item_string() override {
      return MenuNCurses::get_current_item_string();
    };

    void select_down_item() override { MenuNCurses::select_down_item(); };
    void select_up_item() override { MenuNCurses::select_up_item(); };

    char get_input() override { return MenuNCurses::get_input(); };

    void set_border() override {
      MenuNCurses::set_border();
    };

    void unset_border() override {
      MenuNCurses::unset_border();
    };

  private:
    /** @brief Storage for the currently held items. */
    std::vector<T> held_items;

    /** @brief Get display strings from the set of items. */
    std::vector<std::string> items_to_string(const std::vector<T> &items) {
      std::vector<std::string> display_strings;
      for (const auto &it : items) {
        display_strings.push_back(it.name);
      }
      return display_strings;
    };
  };
} // namespace tui

#endif // COLUMN_H
