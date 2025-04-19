#ifndef SUBSTRING_MATCHER_H
#define SUBSTRING_MATCHER_H

#include <string>
#include <vector>
#include <optional>

namespace suggestion {
  class SubstringMatcher {
  public:
    /** @brief Match a string in choices according to the query string
     and the substring matching rules. The empty string is returned if
     no choice is adequate. */
    std::optional<std::size_t> match
    (const std::string &query, const std::vector<std::string> &choices);
  private:
    /** @brief Given a set of strings, return the string which comes first
        in alphabetical order among shortest strings. */
    std::size_t select_string(const std::vector<std::string> &choices,
                              const std::vector<std::size_t> &matches);
  };
}

#endif // SUBSTRING_MATCHER_H
