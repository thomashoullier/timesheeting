#include "weekly_totals.h"

std::vector<std::string> WeeklyTotals::to_strings () const {
  // TODO: have short strings with zeroes and units omitted, and long strings
  //       for status bar display.
  std::vector<std::string> strs;
  // Column headers
  strs.push_back("Task");
  strs.push_back("Mon");
  strs.push_back("Tue");
  strs.push_back("Wed");
  strs.push_back("Thu");
  strs.push_back("Fri");
  strs.push_back("Sat");
  strs.push_back("Sun");
  strs.push_back("TOTAL");
  // Daily totals, all tasks
  strs.push_back("ALL");
  for (auto const &dur : daily_totals) {
    strs.push_back(dur.to_string());
  }
  strs.push_back(total.to_string());
  // Per-project totals
  for (auto const &proj : project_totals) {
    strs.push_back(proj.project_name);
    for (auto const &daily_dur : proj.daily_totals) {
      strs.push_back(daily_dur.to_string());
    }
    strs.push_back(proj.total.to_string());
  }
  return strs;
}
