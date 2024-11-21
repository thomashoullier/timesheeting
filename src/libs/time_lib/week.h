#ifndef WEEK_H
#define WEEK_H

#include "date_range.h"
#include "day.h"
#include <string>
#include <array>
namespace time_lib {
  class Week {
  private:
    /** @brief Internal representation: calendar day of first day
        of the week. */
    std::chrono::year_month_day ymd;

  public:
    /** @brief Initialize the week to the current week
        in the current timezone. */
    Week();
    /** @brief Go to the next week. */
    void next();
    /** @brief Go to the previous week. */
    void previous();
    /** @brief Convert the week to a DateRange. */
    DateRange to_date_range() const;
    /** @brief Return a string representation for the week. */
    std::string to_string() const;
    /** @brief Return the set of Days within the week, in order. */
    std::array<Day, 7> days() const;
  };
}
#endif // WEEK_H
