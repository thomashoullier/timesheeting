#ifndef WEEKLY_TOTALS_H
#define WEEKLY_TOTALS_H

#include "data_objects.h"
#include <array>
#include <string>
#include <vector>

/** @brief Report of totals for a given task. */
struct PerTaskTotals {
  /** @brief Name of the task. */
  std::string task_name;
  /** @brief Weekly total of duration worked on this task. */
  Duration total;
  /** @brief Daily breakdown of the total. */
  std::array<Duration, 7> daily_totals;
};

/** @brief Report of totals for a given project. */
struct PerProjectTotals {
  /** @brief Name of the project. */
  std::string project_name;
  /** @brief Weekly total of duration worked for the project. */
  Duration total;
  /** @brief Daily breakdown of the total, all tasks included. */
  std::array<Duration, 7> daily_totals;
  /** @brief Set of per-task totals. */
  std::vector<PerTaskTotals> task_totals;
};

/** @brief Struct holding the weekly totals for interface between the
    DB and the UI. */
struct WeeklyTotals {
  /** @brief Total duration worked per day of the week, on any task. */
  std::array<Duration, 7> daily_totals;
  /** @brief Set of report per project, including task breakdown. */
  std::vector<PerProjectTotals> project_totals;

  /** @brief Output the weekly totals to a set of strings, in order. */
  std::vector<std::string> to_strings();
};

#endif // WEEKLY_TOTALS_H
