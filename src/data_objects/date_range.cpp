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

void DateRange::add_one_week() {
  start.add_one_week();
  stop.add_one_week();
}

void DateRange::subtract_one_week() {
  start.subtract_one_week();
  stop.subtract_one_week();
}

std::vector<std::string> DateRange::to_string() const {
  std::vector<std::string> strings(2);
  strings.at(0) = start.to_string();
  strings.at(1) = stop.to_string();
  return strings;
}

std::vector<std::string> DateRange::to_day_strings() const {
  std::vector<std::string> strings(2);
  strings.at(0) = start.get_day_string();
  strings.at(1) = stop.get_day_string();
  return strings;
}
