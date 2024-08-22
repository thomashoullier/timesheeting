#include "date_range.h"

DateRange::DateRange(const Date &_start, const Date &_stop)
    : start(_start), stop(_stop) {}

void DateRange::add_one_day() {
  start.add_one_day();
  stop.add_one_day();
}

void DateRange::subtract_one_day() {
  start.subtract_one_day();
  stop.subtract_one_day();
}
