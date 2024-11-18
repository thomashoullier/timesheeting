/** @file
 * @brief StopwatchNcurses definition. */
#ifndef STOPWATCH_NCURSES_H
#define STOPWATCH_NCURSES_H

#include "../entry_field.h"
#include "core/entry_staging.h"
#include "ncurses_lib/menu_ncurses.h"

/** @brief Ncurses implementation of a stopwatch for staging new entries. */
class StopwatchNcurses : public ncurses_lib::MenuNCurses {
public:
  /** @brief Constructor. */
  StopwatchNcurses(const core::EntryStaging &_entry_staging);

  /** @brief Get the field type for the currently selected field. */
  EntryField get_field_type();
  /** @brief Set the current entrystaging item. */
  void set_items(const core::EntryStaging &item);

private:
  /** @brief Entry currently stored in staging. */
  core::EntryStaging entry_staging;
};

#endif // STOPWATCH_NCURSES_H
