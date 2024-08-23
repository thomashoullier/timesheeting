#ifndef STOPWATCH_NCURSES_H
#define STOPWATCH_NCURSES_H

#include "data_objects.h"
#include "menu_ncurses.h"

/** @brief Ncurses implementation of a stopwatch for generating new entries. */
class StopwatchNcurses : public MenuNCurses {
public:
  StopwatchNcurses(const EntryStaging &_entry_staging);

  /** @brief Get the field type for the currently selected field. */
  EntryField get_field_type();
  /** @brief Set the current entrystaging item. */
  void set_items(const EntryStaging &item);

private:
  /** @brief Entry currently stored in staging. */
  EntryStaging entry_staging;
};

#endif // STOPWATCH_NCURSES_H
