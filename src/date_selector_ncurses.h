#ifndef DATE_SELECTOR_NCURSES_H
#define DATE_SELECTOR_NCURSES_H

#include "date_selector_interface.h"
#include <ncurses.h>

class DateSelectorNcurses : public DateSelectorInterface {
private:
  DateRange range;
  /** @brief Handle to the date selector window. */
  WINDOW *win;
  /** @brief Setup the ncurses window for the date selector. */
  WINDOW* init_window();

public:
  /** @brief Initialize the date range to the current day in local time. */
  DateSelectorNcurses ();
  ~DateSelectorNcurses ();
  void print () const override;
  void refresh () const override;
  void clear () const override;
  DateRange current_range () const override;
  void select_next_day () override;
  void select_previous_day () override;
};

#endif // DATE_SELECTOR_NCURSES_H
