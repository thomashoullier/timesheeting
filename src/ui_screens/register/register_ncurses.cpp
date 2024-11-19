#include "register_ncurses.h"

namespace tui {
  RegisterNcurses::RegisterNcurses(const std::vector<core::Entry> &items)
    : MenuNCurses(items_to_string(items), items_to_shortstring(items),
                  ncurses_lib::WindowPosition::upper,
                  ncurses_lib::WindowFormat::block, 5),
      held_items(items) {}

  void RegisterNcurses::set_items(const std::vector<core::Entry> &items) {
    held_items = items;
    MenuNCurses::set_items(items_to_string(items),
                           items_to_shortstring(items));
  }

  core::Id RegisterNcurses::get_current_id() {
    if (held_items.empty()) {
      throw(ncurses_lib::MenuEmpty
            ("get_current_id(): no items in the register!"));
    }
    auto item_index = get_row_index();
    return held_items.at(item_index).id;
  }

  EntryField RegisterNcurses::get_field_type() {
    return EntryField(get_col_index());
  }

  std::vector<std::string>
  RegisterNcurses::items_to_string(const std::vector<core::Entry> &items) {
    std::vector<std::string> display_strings;
    for (const auto &it : items) {
      auto it_strings = it.to_strings();
      display_strings.insert(display_strings.end(), it_strings.begin(),
                             it_strings.end());
    }
    return display_strings;
  }

  std::vector<std::string>
  RegisterNcurses::items_to_shortstring(const std::vector<core::Entry> &items) {
    std::vector<std::string> display_strings;
    for (const auto &it : items) {
      auto it_strings = it.to_shortstrings();
      display_strings.insert(display_strings.end(), it_strings.begin(),
                             it_strings.end());
    }
    return display_strings;
  }
} // namespace tui
