/** @file
 * @brief ProjectTotal definition. */
#ifndef PROJECT_TOTAL_H
#define PROJECT_TOTAL_H

#include "time_lib/duration.h"
#include <string>
#include <vector>

namespace core {
  /** @brief Struct holding a total worked time for a given project. */
  struct ProjectTotal {
    /** @brief The name of the project worked on. */
    std::string project_name;
    /** @brief The Duration worked on the project. */
    time_lib::Duration total;

    /** @brief Get a representation as strings with project name and duration. */
    std::vector<std::string> to_strings () const;
  };
}
#endif // PROJECT_TOTAL_H
