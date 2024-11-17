#ifndef ENTRY_H
#define ENTRY_H

#include "time_lib/date.h"
#include "id.h"

/** @brief Timesheet entry object. */
struct Entry {
  /** @brief Id of the entry. */
  Id id;
  /** @brief Project name associated to the entry. */
  std::string project_name;
  /** @brief Task name associated to the entry. */
  std::string task_name;
  /** @brief Start date of the entry. */
  time_lib::Date start;
  /** @brief Stop date of the entry. */
  time_lib::Date stop;
  /** @brief Location of the entry. */
  std::string location_name;

  /** @brief Generate an ordered set of strings for displaying the entry.

   Typically, the result is like {"Project", "Task",
  "23Aug2024 16:24:05", "23Aug2024 17:35:38", "Office"}. */
  std::vector<std::string> to_strings() const;
  /** @brief Generate an ordered set of strings for displaying the entry
      in short form.

   Typically, the result is like {"Project", "Task", "16:24", "17:35",
                                  "Office"}. */
  std::vector<std::string> to_shortstrings() const;
};

#endif // ENTRY_H
