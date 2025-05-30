/** @file
 * @brief StopwatchNcurses definition. */
#ifndef STOPWATCH_NCURSES_H
#define STOPWATCH_NCURSES_H

#include "../entry_field.h"
#include "core/entry_staging.h"
#include "ncurses_lib/menu_ncurses.h"
#include <memory>
#include <vector>
// IWYU pragma: no_include <format>

namespace tui {
  /** @brief Ncurses implementation of a stopwatch for staging new entries. */
  class StopwatchNcurses final : public ncurses_lib::MenuNCurses {
  public:
    /** @brief Constructor. */
    explicit StopwatchNcurses(const core::EntryStaging &_entry_staging);

    /** @brief Get the field type for the currently selected field. */
    EntryField get_field_type();
    /** @brief Set the current entrystaging item. */
    void set_items(const core::EntryStaging &item);

  private:
    /** @brief Entry currently stored in staging. */
    core::EntryStaging entry_staging;

    /** @brief Convert the EntryStaging object to menu items. */
    std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
    entrystaging_to_menu(const core::EntryStaging &entry_staging);
  };
} // namespace tui

#endif // STOPWATCH_NCURSES_H
