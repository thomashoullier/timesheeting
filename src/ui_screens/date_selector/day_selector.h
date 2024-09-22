#ifndef DAY_SELECTOR_H
#define DAY_SELECTOR_H

#include "../../data_objects/date_range.h"
#include "../../data_objects/day.h"
#include "../../ncurses/bar_ncurses.h"

/** @brief Bar for selecting a calendar day. */
class DaySelector : public BarNCurses {
private:
  /** @brief Internal representation of the day. */
  Day day;

public:
  /** @brief Initialize the day to the current day in local time. */
  DaySelector();
  /** @brief Return the currently selected day as a date range with a resolution
      of 1 second. */
  DateRange current_range() const;
  /** @brief Select the next day. */
  void select_next_day();
  /** @brief Select the previous day. */
  void select_previous_day();
  /** @brief Update the display of the day. */
  void refresh();
};

#endif // DAY_SELECTOR_H
