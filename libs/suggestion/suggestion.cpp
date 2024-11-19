#include "rapidfuzz/fuzz.hpp"

namespace suggestion{
  std::string best_match(const std::string &query,
                         const std::vector<std::string> &choices) {
    double best_score {0.0};
    std::string best_match;
    rapidfuzz::fuzz::CachedPartialTokenSetRatio<std::string::value_type>
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
