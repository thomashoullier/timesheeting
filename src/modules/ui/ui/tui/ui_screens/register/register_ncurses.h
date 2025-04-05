/** @file
 * @brief RegisterNcurses definition. */
#ifndef REGISTER_NCURSES_H
#define REGISTER_NCURSES_H

#include "core/entry.h"
#include "../entry_field.h"
#include "core/id.h"
#include "ncurses_lib/menu_ncurses.h"
#include <memory>
#include <string>
#include <vector>

namespace tui {
  /** @brief Entry register implementation in ncurses.

      A register is a menu with row items made of several columns. */
  class RegisterNcurses final : public ncurses_lib::MenuNCurses {
  public:
    /** @brief Constructor. */
    explicit RegisterNcurses(const std::vector<core::Entry> &items);

    /** @brief Replace the entries held in the register. */
    void set_items(const std::vector<core::Entry> &items);
    /** @brief Get the Id of the currently selected item. */
    core::Id get_current_id();
    /** @brief Get the type of the currently selected column. */
    EntryField get_field_type ();

  private:
    /** @brief Entries stored in the register. */
    std::vector<core::Entry> held_items;

    /** @brief Convert the items to menu items. */
    std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
    items_to_menu (const std::vector<core::Entry> &items);
  };
} // namespace tui

#endif // REGISTER_NCURSES_H
