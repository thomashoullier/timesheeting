/** @file
 *  @brief EntryStaging definition. */
#ifndef ENTRY_STAGING_H
#define ENTRY_STAGING_H

#include <optional>
#include <string>
#include <vector>
#include "time_lib/date.h"

namespace core {
  /** @brief Entry as it is being staged.

      Each field may or may not be currently filled. */
  struct EntryStaging {
    /** @brief Project name associated to the entry in staging. */
    std::optional<std::string> project_name;
    /** @brief Task name associated to the entry in staging. */
    std::optional<std::string> task_name;
    /** @brief Start date of the entry in staging. */
    std::optional<time_lib::Date> start;
    /** @brief Stop date of the entry in staging. */
    std::optional<time_lib::Date> stop;
    /** @brief Location of the entry in staging. */
    std::optional<std::string> location_name;

    /** @brief Generate an ordered set of strings for displaying
        the entrystaging.

        Fields with no values are output as a whitespace character. */
    std::vector<std::string> to_strings() const;
    /** @brief Generate an ordered set of strings for displaying
        the entrystaging in short form.

        Fields with no values are output as a whitespace character. */
    std::vector<std::string> to_shortstrings() const;
  };
}
#endif // ENTRY_STAGING_H
