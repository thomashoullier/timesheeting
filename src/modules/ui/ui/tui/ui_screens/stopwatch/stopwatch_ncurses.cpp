#include "stopwatch_ncurses.h"
#include "core/entry_staging.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include <memory>

namespace tui {
  StopwatchNcurses::StopwatchNcurses(const core::EntryStaging &_entry_staging)
    : MenuNCurses(entrystaging_to_menu(_entry_staging),
                  ncurses_lib::WindowPosition::lower,
                  ncurses_lib::WindowFormat::box, {0, 0, 0, 0, 0}),
      entry_staging(_entry_staging) {}

  EntryField StopwatchNcurses::get_field_type() {
    return EntryField(get_col_index());
  }

  void StopwatchNcurses::set_items(const core::EntryStaging &item) {
    entry_staging = item;
    MenuNCurses::set_items(entrystaging_to_menu(entry_staging));
  }

  std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
  StopwatchNcurses::entrystaging_to_menu
  (const core::EntryStaging &entry_staging) {
    auto menu_items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
    auto cell_strings = entry_staging.to_shortstrings();
    auto display_strings = entry_staging.to_strings();
    for (std::size_t i = 0 ; i < cell_strings.size() ; ++i) {
      menu_items->push_back
        (ncurses_lib::MenuItem(cell_strings.at(i),
                               display_strings.at(i),
                               ncurses_lib::StringFace::Normal));
    }
    return menu_items;
  }
} // namespace tui
