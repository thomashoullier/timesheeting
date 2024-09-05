#include "weekly_totals.h"

std::vector<std::string> WeeklyTotals::to_strings () {
  std::vector<std::string> strs;
  for (auto const &dur : daily_totals) {
    strs.push_back(dur.to_string());
  }
  return strs;
}
