#ifndef DATE_SELECTOR_NCURSES_H
#define DATE_SELECTOR_NCURSES_H

#include "bar_ncurses.h"
#include "date_selector_interface.h"
#include <ncurses.h>

class DateSelectorNcurses : public DateSelectorInterface,
                            public BarNCurses {
private:
  DateRange range;

public:
  /** @brief Initialize the date range to the current day in local time. */
  DateSelectorNcurses ();
  void refresh () const override;
  void clear () const override;
  DateRange current_range () const override;
  void select_next_day () override;
  void select_previous_day () override;
};

#endif // DATE_SELECTOR_NCURSES_H
