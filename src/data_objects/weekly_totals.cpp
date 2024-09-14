#include "weekly_totals.h"

std::vector<std::string> WeeklyTotals::to_strings () const {
  // TODO: have short strings with zeroes and units omitted, and long strings
  //       for status bar display.
  std::vector<std::string> strs;
  // Column headers
  strs.push_back("Project/Task");
  strs.push_back("Monday");
  strs.push_back("Tuesday");
  strs.push_back("Wednesday");
  strs.push_back("Thursday");
  strs.push_back("Friday");
  strs.push_back("Saturday");
  strs.push_back("Sunday");
  strs.push_back("TOTAL");
  // Daily totals, all tasks
  strs.push_back("All tasks");
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
    // Per-task totals
    for (auto const &task : proj.task_totals) {
      strs.push_back(task.task_name);
      for (auto const &daily_dur : task.daily_totals) {
        strs.push_back(daily_dur.to_string());
      }
      strs.push_back(task.total.to_string());
    }
  }
  return strs;
}

std::vector<StringWithFace> WeeklyTotals::to_shortstrings() const {
  std::vector<StringWithFace> strs;
  // Column headers
  strs.push_back(StringWithFace("Task"));
  strs.push_back(StringWithFace("Mon"));
  strs.push_back(StringWithFace("Tue"));
  strs.push_back(StringWithFace("Wed"));
  strs.push_back(StringWithFace("Thu"));
  strs.push_back(StringWithFace("Fri"));
  strs.push_back(StringWithFace("Sat"));
  strs.push_back(StringWithFace("Sun"));
  strs.push_back(StringWithFace("TOTAL"));
  // Daily totals, all tasks
  strs.push_back(StringWithFace("ALL"));
  for (auto const &dur : daily_totals) {
    strs.push_back(dur.to_shortstring());
  }
  strs.push_back(total.to_shortstring());
  // Per-project totals
  for (auto const &proj : project_totals) {
    strs.push_back(StringWithFace(proj.project_name, true));
    for (auto const &daily_dur : proj.daily_totals) {
      strs.push_back(StringWithFace(daily_dur.to_shortstring(), true));
    }
    strs.push_back(StringWithFace(proj.total.to_shortstring(), true));
    // Per-task totals
    for (auto const &task : proj.task_totals) {
      strs.push_back(task.task_name);
      for (auto const &daily_dur : task.daily_totals) {
        strs.push_back(daily_dur.to_shortstring());
      }
      strs.push_back(task.total.to_shortstring());
    }
  }
  return strs;
}
