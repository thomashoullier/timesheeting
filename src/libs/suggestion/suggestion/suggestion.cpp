#include "suggestion.h"
#include "fuzzy_matcher.h"
#include "substring_matcher.h"
#include <cctype>
#include <optional>
#include <vector>
#include <string>
#include <algorithm>

namespace suggestion{
  std::string best_match(const std::string &query,
                         const std::vector<std::string> &choices) {
    if (choices.empty() or query.empty())
      return "";
    // Simple substring matcher try.
    auto substring_matcher = SubstringMatcher();
    std::optional<std::size_t> substring_match =
        substring_matcher.match(query, choices);
    if (substring_match.has_value()) {
      auto ind = substring_match.value();
      return choices.at(ind);
    }
    // Remove the case from the query and choices
    std::string query_nocase = string_tolower(query);
    std::vector<std::string> choices_nocase;
    choices_nocase.reserve(choices.size());
    std::transform(choices.begin(), choices.end(),
                   std::back_inserter(choices_nocase),
                   [](const std::string &str) { return string_tolower(str); });
    std::optional<std::size_t> substring_match_nocase =
        substring_matcher.match(query_nocase, choices_nocase);
    if (substring_match_nocase.has_value()) {
      auto ind = substring_match_nocase.value();
      return choices.at(ind);
    }
    // Else, fuzzy match.
    auto fuzzy_matcher = FuzzyMatcher();
    return fuzzy_matcher.match(query, choices);
  }

  std::string string_tolower(const std::string &str) {
    std::string lower_str = str;
    std::transform(str.begin(), str.end(), lower_str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lower_str;
  }
}
