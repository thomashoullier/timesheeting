#ifndef WEEKLY_TOTALS_H
#define WEEKLY_TOTALS_H

#include "data_objects.h"
#include <array>
#include <vector>

/** @brief Struct holding the weekly totals for interface between the
    DB and the UI. */
struct WeeklyTotals {
  /** @brief Total duration worked per day of the week, on any task. */
  std::array<Duration, 7> daily_totals;

  /** @brief Output the weekly totals to a set of strings, in order. */
  std::vector<std::string> to_strings ();
};

#endif // WEEKLY_TOTALS_H
