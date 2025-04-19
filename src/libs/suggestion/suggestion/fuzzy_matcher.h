/** @file
    @brief FuzzyMatcher definition. */
#ifndef FUZZY_MATCHER_H
#define FUZZY_MATCHER_H

#include <string>
#include <vector>

namespace suggestion {
  class FuzzyMatcher {
  public:
    std::string match (const std::string &query,
                       const std::vector<std::string> &choices);
  };
} // namespace suggestion

#endif // FUZZY_MATCHER_H
