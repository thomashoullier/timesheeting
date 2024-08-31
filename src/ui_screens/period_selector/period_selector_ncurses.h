#ifndef PERIOD_SELECTOR_NCURSES_H
#define PERIOD_SELECTOR_NCURSES_H

#include "../../ncurses/menu_ncurses.h"
#include "../../data_objects/date_range.h"

class PeriodSelectorNCurses : public MenuNCurses {
public:
  /** @brief Constructor. */
  PeriodSelectorNCurses(const DateRange &_period);

private:
  /** @brief Currently selected period. */
  DateRange period;
};

#endif // PERIOD_SELECTOR_NCURSES_H
