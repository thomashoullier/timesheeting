/** @file
 * @brief ProjectTotal definition. */
#ifndef PROJECT_TOTAL_H
#define PROJECT_TOTAL_H

#include "time_lib/duration.h"
#include <string>
#include <vector>

/** @brief Struct holding a total worked time for a given project. */
struct ProjectTotal {
  std::string project_name;
  time_lib::Duration total;

  std::vector<std::string> to_strings () const;
};

#endif // PROJECT_TOTAL_H
