/** @file
    @brief Day definition. */
#ifndef DAY_H
#define DAY_H

#include <chrono>
#include <string>
#include "date_range.h"

namespace time_lib {
  /** @brief Local calendar day class. */
  class Day {
  private:
    /** @brief Internal representation of the day. */
    std::chrono::year_month_day ymd;

  public:
    /** @brief Initialize to the current day in the current timezone. */
    explicit Day();
    /** @brief Initialize the day using a year_month_day. */
    explicit Day(std::chrono::year_month_day _ymd);
    /** @brief Go to the next day. */
    void next();
    /** @brief Go to the previous day. */
    void previous();
    /** @brief Convert the day to a DateRange with a second resolution. */
    DateRange to_date_range() const;
    /** @brief Return a string representation of the day. */
    std::string to_string() const;
    /** @brief Return a day-month-year string representation of the day. */
    std::string to_day_month_year_string() const;
    /** @brief Whether the day contains the given date. */
    bool contains(const Date &date) const;
  };
}
#endif // DAY_H
