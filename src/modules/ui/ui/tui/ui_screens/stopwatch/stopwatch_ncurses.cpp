#include "stopwatch_ncurses.h"

namespace tui {
  StopwatchNcurses::StopwatchNcurses(const core::EntryStaging &_entry_staging)
    : MenuNCurses(_entry_staging.to_strings(), _entry_staging.to_shortstrings(),
                  ncurses_lib::WindowPosition::lower,
                  ncurses_lib::WindowFormat::box, 5),
      entry_staging(_entry_staging) {}

  EntryField StopwatchNcurses::get_field_type() {
    return EntryField(get_col_index());
  }

  void StopwatchNcurses::set_items(const core::EntryStaging &item) {
    entry_staging = item;
    MenuNCurses::set_items(entry_staging.to_strings(),
                           entry_staging.to_shortstrings());
  }
} // namespace tui
