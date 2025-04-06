#include <rapidfuzz/fuzz.hpp>
// IWYU pragma: no_include <rapidfuzz/details/Range.hpp>
// IWYU pragma: no_include <rapidfuzz/details/common_impl.hpp>
// IWYU pragma: no_include <rapidfuzz/fuzz_impl.hpp>
// IWYU pragma: no_include <format>
// IWYU pragma: no_include <iterator>
#include <vector>
#include <string>

namespace suggestion{
  std::string best_match(const std::string &query,
                         const std::vector<std::string> &choices) {
    double best_score {0.0};
    std::string best_match;
    rapidfuzz::fuzz::CachedTokenSortRatio<std::string::value_type>
      scorer(query);
    for (const auto &choice : choices) {
      double score = scorer.similarity(choice, best_score);
      if (score >= best_score) {
        best_score = score;
        best_match = choice;
      }
    }
    return best_match;
  }
}
