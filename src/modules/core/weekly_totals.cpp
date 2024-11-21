#include "weekly_totals.h"
#include "time_lib/duration_displayer.h"

namespace core {
  std::vector<std::string> WeeklyTotals::to_strings () const {
    // TODO: have short strings with zeroes and units omitted, and long strings
    //       for status bar display.
    std::vector<std::string> strs;
    auto duration_formatter = time_lib::DurationDisplayer::get();
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
      strs.push_back(duration_formatter.to_string(dur));
    }
    strs.push_back(duration_formatter.to_string(total));
    // Per-project totals
    for (auto const &proj : project_totals) {
      strs.push_back(proj.project_name);
      for (auto const &daily_dur : proj.daily_totals) {
        strs.push_back(duration_formatter.to_string(daily_dur));
      }
      strs.push_back(duration_formatter.to_string(proj.total));
      // Per-task totals
      for (auto const &task : proj.task_totals) {
        strs.push_back(task.task_name);
        for (auto const &daily_dur : task.daily_totals) {
          strs.push_back(duration_formatter.to_string(daily_dur));
        }
        strs.push_back(duration_formatter.to_string(task.total));
      }
    }
    return strs;
  }

  std::vector<ncurses_lib::StringWithFace>
  WeeklyTotals::to_shortstrings() const {
    std::vector<ncurses_lib::StringWithFace> strs;
    auto duration_formatter = time_lib::DurationDisplayer::get();
    // Column headers
    strs.push_back(ncurses_lib::StringWithFace("Task"));
    strs.push_back(ncurses_lib::StringWithFace("Mon"));
    strs.push_back(ncurses_lib::StringWithFace("Tue"));
    strs.push_back(ncurses_lib::StringWithFace("Wed"));
    strs.push_back(ncurses_lib::StringWithFace("Thu"));
    strs.push_back(ncurses_lib::StringWithFace("Fri"));
    strs.push_back(ncurses_lib::StringWithFace("Sat"));
    strs.push_back(ncurses_lib::StringWithFace("Sun"));
    strs.push_back(ncurses_lib::StringWithFace("TOTAL"));
    // Daily totals, all tasks
    strs.push_back(ncurses_lib::StringWithFace("ALL"));
    for (auto const &dur : daily_totals) {
      strs.push_back(duration_formatter.to_shortstring(dur));
    }
    strs.push_back(duration_formatter.to_shortstring(total));
    // Per-project totals
    for (auto const &proj : project_totals) {
      strs.push_back(ncurses_lib::StringWithFace(proj.project_name, true));
      for (auto const &daily_dur : proj.daily_totals) {
        strs.push_back(ncurses_lib::StringWithFace
                       (duration_formatter.to_shortstring(daily_dur), true));
      }
      strs.push_back(ncurses_lib::StringWithFace
                     (duration_formatter.to_shortstring(proj.total), true));
      // Per-task totals
      for (auto const &task : proj.task_totals) {
        strs.push_back(task.task_name);
        for (auto const &daily_dur : task.daily_totals) {
          strs.push_back(duration_formatter.to_shortstring(daily_dur));
        }
        strs.push_back(duration_formatter.to_shortstring(task.total));
      }
    }
    return strs;
  }
}
