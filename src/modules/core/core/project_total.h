/** @file
 * @brief ProjectTotal definition. */
#ifndef PROJECT_TOTAL_H
#define PROJECT_TOTAL_H

#include "ncurses_lib/menu_ncurses.h"
#include "time_lib/duration.h"
#include <string>
#include <vector>
// IWYU pragma: no_include <format>

namespace core {
  /** @brief List of task names and associated durations. */
  struct TaskTotal {
    /** @brief The name of the task. */
    std::string task_name;
    /** @brief The duration total associated to the task. */
    time_lib::Duration total;
  };

  /** @brief Struct holding a total worked time for a given project
             and the corresponding list of totals for all tasks. */
  struct ProjectTotal {
    /** @brief The name of the project worked on. */
    std::string project_name;
    /** @brief The Duration worked on the project. */
    time_lib::Duration total;
    /** @brief List of task totals. */
    std::vector<TaskTotal> task_totals;

    /** @brief Get a representation as a list of MenuItem. */
    std::vector<ncurses_lib::MenuItem> to_menu_items() const;
  };
}
#endif // PROJECT_TOTAL_H
