/** @file
 * @brief Date range definition. */
#ifndef DATE_RANGE_H
#define DATE_RANGE_H

#include "date.h"
#include <vector>

namespace time_lib{
  /** @brief Class for representing a range between a start and stop dates . */
  class DateRange {
  public:
    /** @brief Start of the date range. */
    Date start;
    /** @brief End of the date range. */
    Date stop;

    /** @brief Initialize the date range. */
    DateRange(const Date &_start, const Date &_stop);

    /** @brief Get the date range as full strings. */
    std::vector<std::string> to_string() const;
    /** @brief Get thet date range as short strings with only the day. */
    std::vector<std::string> to_day_strings() const;
  };
}
#endif // DATE_RANGE_H
