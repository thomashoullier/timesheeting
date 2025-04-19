/** @file
 *  @brief Suggestion engine utility. */
#ifndef SUGGESTION_H
#define SUGGESTION_H

#include <vector>
#include <string>

namespace suggestion {
  /** @brief Suggest the best match given a query and a list of choices. */
  std::string best_match (const std::string &query,
                          const std::vector<std::string> &choices);
  /** @brief Helper function: convert a string to lowercase. */
  std::string string_tolower (const std::string &str);
} // namespace suggestion

#endif // SUGGESTION_H
