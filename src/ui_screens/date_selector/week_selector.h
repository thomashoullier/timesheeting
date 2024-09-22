#ifndef WEEK_SELECTOR_H
#define WEEK_SELECTOR_H

#include "../../data_objects/week.h"
#include "../../data_objects/date_range.h"
#include "../../ncurses/bar_ncurses.h"

class WeekSelector : public BarNCurses {
private:
  /** @brief Internal representation of current week. */
  Week week;

public:
  /** @brief Initialize the day to the first day of the week in local time. */
  WeekSelector();
  /** @brief Return the currently selected week. */
  Week current_week() const;
  /** @brief Return the current week as a DateRange. */
  DateRange current_range() const;
  /** @brief Select the next week. */
  void select_next_week();
  /** @brief Select the previous week. */
  void select_previous_week();
  /** @brief Update the display. */
  void refresh();
};

#endif // WEEK_SELECTOR_H
