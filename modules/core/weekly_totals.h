#ifndef WEEKLY_TOTALS_H
#define WEEKLY_TOTALS_H

#include "time_lib/duration.h"
#include "ncurses_lib/string_with_face.h"
#include <array>
#include <string>
#include <vector>

namespace core {
  /** @brief Report of totals for a given task. */
  struct PerTaskTotals {
    /** @brief Name of the task. */
    std::string task_name;
    /** @brief Weekly total of duration worked on this task. */
    time_lib::Duration total;
    /** @brief Daily breakdown of the total. */
    std::array<time_lib::Duration, 7> daily_totals;
  };

  /** @brief Report of totals for a given project. */
  struct PerProjectTotals {
    /** @brief Name of the project. */
    std::string project_name;
    /** @brief Weekly total of duration worked for the project. */
    time_lib::Duration total;
    /** @brief Daily breakdown of the total, all tasks included. */
    std::array<time_lib::Duration, 7> daily_totals;
    /** @brief Set of per-task totals. */
    std::vector<PerTaskTotals> task_totals;
  };

  /** @brief Struct holding the weekly totals for interface between the
      DB and the UI. */
  struct WeeklyTotals {
    /** @brief Total duration worked during the week, on any task. */
    time_lib::Duration total;
    /** @brief Total duration worked per day of the week, on any task. */
    std::array<time_lib::Duration, 7> daily_totals;
    /** @brief Set of report per project, including task breakdown. */
    std::vector<PerProjectTotals> project_totals;

    /** @brief Output the weekly totals to a set of strings, in order. */
    std::vector<std::string> to_strings() const;
    /** @brief Output the weekly totals to a set of shortstrings, in order. */
    std::vector<ncurses_lib::StringWithFace> to_shortstrings() const;
  };
}
#endif // WEEKLY_TOTALS_H
