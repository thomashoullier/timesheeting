#include "period_selector_ncurses.h"

PeriodSelectorNCurses::PeriodSelectorNCurses(const DateRange &_period)
  : MenuNCurses(_period.to_string(), _period.to_day_strings(),
                WindowPosition::lower, WindowFormat::box, 2),
    period(_period) {}
