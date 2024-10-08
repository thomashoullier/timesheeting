/** @file
 * @brief RegisterNcurses definition. */
#ifndef REGISTER_NCURSES_H
#define REGISTER_NCURSES_H

#include "../../data_objects/entry.h"
#include "../../data_objects/entry_field.h"
#include "../../data_objects/id.h"
#include "../../ncurses/menu_ncurses.h"
#include <string>
#include <vector>

/** @brief Entry register implementation in ncurses.

 A register is a menu with row items made of several columns. */
class RegisterNcurses : public MenuNCurses {
public:
  /** @brief Constructor. */
  RegisterNcurses(const std::vector<Entry> &items);

  /** @brief Replace the entries held in the register. */
  void set_items(const std::vector<Entry> &items);
  /** @brief Get the Id of the currently selected item. */
  Id get_current_id();
  /** @brief Get the type of the currently selected column. */
  EntryField get_field_type ();

private:
  /** @brief Entries stored in the register. */
  std::vector<Entry> held_items;

  /** @brief Get display strings from the provided set of items. */
  std::vector<std::string> items_to_string(const std::vector<Entry> &items);
  /** @brief Get short display strings from the provided set of items. */
  std::vector<std::string> items_to_shortstring
  (const std::vector<Entry> &items);
};
#endif // REGISTER_NCURSES_H
