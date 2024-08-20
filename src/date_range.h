#ifndef DATE_RANGE_H
#define DATE_RANGE_H

#include "date.h"

/** @brief Class for representing a range of Dates. */
class DateRange {
public:
  /** @brief Start of the date range. */
  Date start;
  /** @brief End of the date range. */
  Date stop;

  /** @brief Initialize the date range. */
  DateRange(const Date &_start, const Date &_stop):
    start(_start), stop(_stop) {};
};
#endif // DATE_RANGE_H
