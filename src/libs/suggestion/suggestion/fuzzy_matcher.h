/** @file
    @brief FuzzyMatcher definition. */
#ifndef FUZZY_MATCHER_H
#define FUZZY_MATCHER_H

#include <string>
#include <vector>

namespace suggestion {
  /** @brief Matcher with fuzzy behavior. */
  class FuzzyMatcher {
  public:
    /** @brief Match a string in choices according to the query string
     and the fuzzy matching rules. */
    std::string match (const std::string &query,
                       const std::vector<std::string> &choices);
  };
} // namespace suggestion

#endif // FUZZY_MATCHER_H
