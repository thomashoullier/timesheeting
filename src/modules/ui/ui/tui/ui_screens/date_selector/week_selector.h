/** @file
 *  @brief WeekSelector definition. */
#ifndef WEEK_SELECTOR_H
#define WEEK_SELECTOR_H

#include "time_lib/week.h"
#include "time_lib/date_range.h"
#include "ncurses_lib/bar_ncurses.h"

namespace tui {
  /** @brief UI for specifying a week. */
  class WeekSelector : public ncurses_lib::BarNCurses {
  private:
    /** @brief Internal representation of current week. */
    time_lib::Week week;

  public:
    /** @brief Initialize the day to the first day of the week in local time. */
    explicit WeekSelector();
    /** @brief Return the currently selected week. */
    time_lib::Week current_week() const;
    /** @brief Return the current week as a DateRange. */
    time_lib::DateRange current_range() const;
    /** @brief Select the next week. */
    void select_next_week();
    /** @brief Select the previous week. */
    void select_previous_week();
    /** @brief Update the display. */
    void refresh() final;
  };
} // namespace tui

#endif // WEEK_SELECTOR_H
