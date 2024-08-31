#include "period_selector_ncurses.h"

PeriodSelectorNCurses::PeriodSelectorNCurses(const DateRange &_period)
  : MenuNCurses(_period.to_string(), _period.to_day_strings(),
                WindowPosition::lower, WindowFormat::box, 2),
    period(_period) {}

PeriodSelectorNCurses::PeriodField PeriodSelectorNCurses::get_field_type() {
  return PeriodField(get_col_index());
}

void PeriodSelectorNCurses::set_start(const Date &new_start_date) {
  period.start = new_start_date;
  this->update();
}

void PeriodSelectorNCurses::set_stop(const Date &new_stop_date) {
  period.stop = new_stop_date;
  this->update();
}

void PeriodSelectorNCurses::update() {
  MenuNCurses::set_items(period.to_string(),
                         period.to_day_strings());
}
