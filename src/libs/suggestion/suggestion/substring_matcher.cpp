#include "substring_matcher.h"
#include <cstddef>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>

namespace suggestion {
  std::optional<std::size_t> SubstringMatcher::match
  (const std::string &query, const std::vector<std::string> &choices) {
    // Index at which the substring query matches in every choice.
    std::vector<std::string::size_type> matching_positions;
    matching_positions.reserve(choices.size());
    std::transform(choices.begin(), choices.end(),
                   std::back_inserter(matching_positions),
                   [&query](const std::string &choice_str) {
                     return choice_str.find(query); });
    // Minimum position at which a match was made.
    std::string::size_type first_pos =
      *std::min_element(matching_positions.begin(),
                        matching_positions.end());
    if (first_pos == std::string::npos) // No match at all
      return {};
    // Set of string indices which contain the earliest match
    std::vector<std::size_t> matching_choices;
    for (std::size_t i = 0 ; i < matching_positions.size() ; ++i) {
      if (first_pos == matching_positions.at(i)) {
        matching_choices.push_back(i);
      }
    }
    if (matching_choices.size() == 1)
      return matching_choices.front();
    return select_string(choices, matching_choices);
  }

  std::size_t SubstringMatcher::select_string
  (const std::vector<std::string> &choices,
   const std::vector<std::size_t> &matches) {
    std::vector<std::string::size_type> string_sizes;
    string_sizes.reserve(matches.size());
    std::transform(matches.begin(), matches.end(),
                   std::back_inserter(string_sizes),
                   [&choices](std::size_t match_ind) {
                     return choices.at(match_ind).size(); });
    std::string::size_type smallest_size =
      *std::min_element(string_sizes.begin(), string_sizes.end());
    // Set of smallest strings
    std::vector<std::size_t> smallest_strings;
    for (std::size_t i = 0 ; i < matches.size() ; ++i) {
      if (string_sizes.at(i) == smallest_size) {
        smallest_strings.push_back(matches.at(i));
      }
    }
    if (smallest_strings.size() == 1)
      return smallest_strings.front();
    // Return the first string in alphabetical order.
    std::vector<std::pair<std::size_t, std::string>> sorted_strings;
    sorted_strings.reserve(smallest_strings.size());
    std::transform(smallest_strings.begin(), smallest_strings.end(),
                   std::back_inserter(sorted_strings),
                   [&choices](std::size_t i) {
                     return std::make_pair(i, choices.at(i)); });
    std::sort(sorted_strings.begin(), sorted_strings.end(),
              [](std::pair<std::size_t, std::string> &p1,
                 std::pair<std::size_t, std::string> &p2) {
                return p1.second < p2.second;});
    return sorted_strings.front().first;
  }
}
