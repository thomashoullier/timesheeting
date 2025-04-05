/** @file
 * @brief Column definition. */
#ifndef COLUMN_H
#define COLUMN_H

#include "column_base.h"
#include "core/generic_item.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include <memory>
#include <type_traits>

namespace tui {
  /** @brief Column menu class. Can hold any GenericItem. */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<core::GenericItem,
                                                          T>>>
  class Column final : public ColumnBase, public ncurses_lib::MenuNCurses {
  public:
    /** @brief Construct the column with given items and position. */
    explicit Column(const std::vector<T> &items,
                    ncurses_lib::WindowPosition winpos)
      : MenuNCurses(items_to_menu(items), winpos,
                    ncurses_lib::WindowFormat::column, {0}),
        held_items(items) {};

    /** @brief Replace the column items. */
    void set_items(const std::vector<T> &items) {
      held_items = items;
      MenuNCurses::set_items(items_to_menu(items));
    };

    core::Id get_current_id() override {
      if (held_items.empty()) {
        throw(ncurses_lib::MenuEmpty
              ("get_current_id(): no items in the register!"));
      }
      auto item_index = get_item_index();
      return held_items.at(item_index).id;
    };

    std::string get_current_item_string() override {
      return MenuNCurses::get_current_item_string();
    };

    ncurses_lib::MenuNCurses::ItemSelectionStatus
    select_down_item() override {
      return MenuNCurses::select_down_item();
    };

    ncurses_lib::MenuNCurses::ItemSelectionStatus
    select_up_item() override {
      return MenuNCurses::select_up_item();
    };

    int get_input() override { return MenuNCurses::get_input(); };

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
    std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
    items_to_menu(const std::vector<T> &items) {
      auto menu_items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
      for (const auto &it : items) {
        if (it.active)
          menu_items->push_back(ncurses_lib::MenuItem(
              it.name, it.name, ncurses_lib::StringFace::Normal));
        else // Inactive items in italic
          menu_items->push_back(ncurses_lib::MenuItem(
              it.name, it.name, ncurses_lib::StringFace::Italic));
      }
      return menu_items;
    };
  };
} // namespace tui

#endif // COLUMN_H
