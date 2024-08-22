#ifndef REGISTER_NCURSES_H
#define REGISTER_NCURSES_H

#include <cstddef>
#include <ncurses.h>
#include <menu.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <exception>
#include "data_objects.h"
#include "menu_ncurses.h"
#include "win_ncurses.h"

/** @brief Entry register implementation in ncurses.

 A register is a menu with row item made of several columns. */
class RegisterNcurses : public MenuNCurses {
  //TODO: do a cpp file
public:
  RegisterNcurses(const std::vector<Entry> &items) :
    MenuNCurses(items_to_string(items),
                WindowPosition::upper, WindowFormat::block, 4),
    held_items(items) {};

  void set_items(const std::vector<Entry> &items) {
    held_items = items; // TODO: memory leak?
    // held_items.clear();
    // held_items.resize(items.size());
    // for (std::size_t i = 0; i < items.size(); ++i) {
    //   held_items.at(i) = items.at(i);
    // }
    MenuNCurses::set_items(items_to_string(items));
  };

  Id get_current_id() {
    if (held_items.empty()) {
      // TODO: throw a column empty exception.
      return 1;
    }
    auto item_index = get_row_index();
    return held_items.at(item_index).id;
  }

  EntryField get_field_type () {
    return EntryField(0); // TEMP
    return EntryField(get_col_index());
  }

  std::string get_current_cell_string () {
    //TODO: use the strings stored in MenuNCurses instead?
    //      This should be a method in MenuNCurses no?
    if (held_items.size() > 0) {
      auto it_index = get_row_index();
      auto field_type = get_field_type();
      auto *it = &held_items.at(it_index);
      switch(field_type) {
      case EntryField::project_name:
        return it->project_name;
        break;
      case EntryField::task_name:
        return it->task_name;
        break;
      case EntryField::start:
        return it->start.str;
        break;
      case EntryField::stop:
        return it->stop.str;
        break;
      default:
        throw std::logic_error("Unknown index for an Entry field.");
      }
    } else {
      return "";
    }
  }

private:
  std::vector<Entry> held_items;

  /** @brief Get display strings from the set of items. */
  std::vector<std::string> items_to_string(const std::vector<Entry> &items) {
    // TODO: operate on held_items directly.
    // TODO: write a utility in Entry to convert it to strings directly.
    std::vector<std::string> display_strings;
    display_strings.reserve(items.size()*4);
    for (const auto &it : items) {
      display_strings.push_back(it.project_name);
      display_strings.push_back(it.task_name);
      display_strings.push_back(it.start.str);
      display_strings.push_back(it.stop.str);
    }
    return display_strings;
  };
};

#endif // REGISTER_NCURSES_H
