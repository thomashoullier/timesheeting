#include "register_ncurses.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include <memory>
#include <cstddef>
// IWYU pragma: no_include <string>
#include "ui/tui/ui_screens/entry_field.h"

namespace tui {
  RegisterNcurses::RegisterNcurses(const std::vector<core::Entry> &items)
    : MenuNCurses(items_to_menu(items),
                  ncurses_lib::WindowPosition::upper,
                  ncurses_lib::WindowFormat::block, {0, 0, 5, 5, 0}),
      held_items(items) {}

  void RegisterNcurses::set_items(const std::vector<core::Entry> &items) {
    held_items = items;
    MenuNCurses::set_items(items_to_menu(items));
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

  bool RegisterNcurses::is_empty() { return held_items.empty(); }

  std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
  RegisterNcurses::items_to_menu(const std::vector<core::Entry> &items) {
    auto menu_items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
    for (const auto &it : items) {
      auto it_strings = it.to_strings();
      auto it_cellstrings = it.to_shortstrings();
      for (std::size_t i = 0 ; i < it_strings.size() ; ++i) {
        menu_items->push_back
          (ncurses_lib::MenuItem(it_cellstrings.at(i), it_strings.at(i),
                                 ncurses_lib::StringFace::Normal));
      }
    }
    return menu_items;
  }
} // namespace tui
