#ifndef DAY_H
#define DAY_H

#include <chrono>
#include "date_range.h"

/** @brief Local calendar day class. */
class Day {
private:
  std::chrono::year_month_day ymd;

public:
  /** @brief Initialize to the current day in the current timezone. */
  Day();
  /** @brief Go to the next day. */
  void next();
  /** @brief Go to the previous day. */
  void previous();
  /** @brief Convert the day to a DateRange with a second resolution. */
  DateRange to_date_range() const;
  /** @brief Return a string representation of the day. */
  std::string to_string() const;
};

#endif // DAY_H
