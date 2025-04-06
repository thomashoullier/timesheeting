/** @file
 *  @brief WeeklyTotals definition. */
#ifndef WEEKLY_TOTALS_H
#define WEEKLY_TOTALS_H

#include "ncurses_lib/menu_ncurses.h"
#include "time_lib/duration.h"
#include <array>
#include <string>
#include <vector>
#include<memory>
// IWYU pragma: no_include <format>

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

    std::shared_ptr<std::vector<ncurses_lib::MenuItem>> to_menu_items() const;

  private:
    /** @brief Convert a duration to a menu item in Normal face. */
    static ncurses_lib::MenuItem
    duration_to_menu_item(const time_lib::Duration &dur);
    /** @brief Convert a duration to a menu item in Bold face. */
    static ncurses_lib::MenuItem
    duration_to_bold_menu_item(const time_lib::Duration &dur);
  };
}
#endif // WEEKLY_TOTALS_H
