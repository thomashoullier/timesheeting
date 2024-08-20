#ifndef DATE_RANGE_H
#define DATE_RANGE_H

#include "date.h"

/** @brief Class for representing a range of Dates. */
class DateRange {
private:
  Date start;
  Date stop;

public:
  /** @brief Initialize the date range. */
  DateRange(const Date &_start, const Date &_stop):
    start(_start), stop(_stop) {};
};

#endif // DATE_RANGE_H
