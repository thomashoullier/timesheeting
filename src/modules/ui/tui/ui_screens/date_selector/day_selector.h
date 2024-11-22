/** @file
 *  @brief DaySelector definition. */
#ifndef DAY_SELECTOR_H
#define DAY_SELECTOR_H

#include "time_lib/date_range.h"
#include "time_lib/day.h"
#include "ncurses_lib/bar_ncurses.h"

namespace tui {
  /** @brief Bar for selecting a calendar day. */
  class DaySelector : public ncurses_lib::BarNCurses {
    // TODO: Factorize with WeekSelector with a template on day/week.
  private:
    /** @brief Internal representation of the day. */
    time_lib::Day day;

  public:
    /** @brief Initialize the day to the current day in local time. */
    DaySelector();
    /** @brief Return the currently selected day as a date range with a resolution
        of 1 second. */
    time_lib::DateRange current_range() const;
    /** @brief Select the next day. */
    void select_next_day();
    /** @brief Select the previous day. */
    void select_previous_day();
    /** @brief Update the display of the day. */
    void refresh();
  };
} // namespace tui

#endif // DAY_SELECTOR_H
