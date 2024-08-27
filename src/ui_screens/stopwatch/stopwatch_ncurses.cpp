#include "stopwatch_ncurses.h"

StopwatchNcurses::StopwatchNcurses(const EntryStaging &_entry_staging)
  : MenuNCurses(_entry_staging.to_strings(), _entry_staging.to_shortstrings(),
                WindowPosition::lower, WindowFormat::box, 4),
    entry_staging(_entry_staging) {}

EntryField StopwatchNcurses::get_field_type() {
  return EntryField(get_col_index());
}

void StopwatchNcurses::set_items(const EntryStaging &item) {
  entry_staging = item;
  MenuNCurses::set_items(entry_staging.to_strings(),
                         entry_staging.to_shortstrings());
}
