/** @file
 * @brief Date range definition. */
#ifndef DATE_RANGE_H
#define DATE_RANGE_H

#include "date.h"

/** @brief Class for representing a range between a start and stop dates . */
class DateRange {
public:
  /** @brief Start of the date range. */
  Date start;
  /** @brief End of the date range. */
  Date stop;

  /** @brief Initialize the date range. */
  DateRange(const Date &_start, const Date &_stop);

  /** @brief Shift the date range by adding one day. */
  void add_one_day();
  /** @brief Shift the date range by subtracting one day. */
  void subtract_one_day();
};
#endif // DATE_RANGE_H
