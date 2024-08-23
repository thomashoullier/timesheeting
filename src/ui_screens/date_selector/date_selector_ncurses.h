#ifndef DATE_SELECTOR_NCURSES_H
#define DATE_SELECTOR_NCURSES_H

#include "../../ncurses/bar_ncurses.h"
#include "../../data_objects/date_range.h"

class DateSelectorNcurses : public BarNCurses {
private:
  DateRange range;

public:
  /** @brief Initialize the date range to the current day in local time. */
  DateSelectorNcurses ();
  /** @brief Refresh the display. */
  void refresh () const;
  /** @brief Clear the display. */
  void clear() const;
  /** @brief Return the currently selected date range. */
  DateRange current_range() const;
  /** @brief Select the next day. */
  void select_next_day();
  /** @brief Select the previous day. */
  void select_previous_day();
};

#endif // DATE_SELECTOR_NCURSES_H
