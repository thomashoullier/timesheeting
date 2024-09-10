/** @file
 * @brief DateSelectorNcurses definition. */
#ifndef DATE_SELECTOR_NCURSES_H
#define DATE_SELECTOR_NCURSES_H

#include "../../ncurses/bar_ncurses.h"
#include "../../data_objects/date_range.h"

/** @brief Bar for selecting a date range. */
class DateSelectorNcurses : public BarNCurses {
private:
  /** @brief Internal representation of the date range. */
  DateRange range;

public:
  /** @brief Initialize the date range to the current day in local time. */
  DateSelectorNcurses ();
  /** @brief Initialize the date range to a given one. */
  DateSelectorNcurses (const DateRange &date_range);
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
  /** @brief Select the next week. */
  void select_next_week();
  /** @brief Select the previous week. */
  void select_previous_week();
};

#endif // DATE_SELECTOR_NCURSES_H
