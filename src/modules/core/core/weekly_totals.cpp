#include "weekly_totals.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include "time_lib/duration_displayer.h"
#include <memory>

namespace core {
  std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
  WeeklyTotals::to_menu_items() const {
    auto menu_items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
    auto duration_formatter = time_lib::DurationDisplayer::get();
    // Headers
    menu_items->push_back(ncurses_lib::MenuItem("Task", "Project/Task",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("Mon", "Monday",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("Tue", "Tuesday",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("Wed", "Wednesday",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("Thu", "Thursday",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("Fri", "Friday",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("Sat", "Saturday",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("Sun", "Sunday",
                                               ncurses_lib::StringFace::Normal));
    menu_items->push_back(ncurses_lib::MenuItem("TOTAL", "TOTAL",
                                               ncurses_lib::StringFace::Normal));
    // Daily totals, all tasks
    menu_items->push_back(ncurses_lib::MenuItem("ALL", "All tasks",
                                               ncurses_lib::StringFace::Normal));
    for (auto const &dur : daily_totals) {
      menu_items->push_back(
          ncurses_lib::MenuItem(duration_formatter.to_shortstring(dur),
                                duration_formatter.to_string(dur),
                                ncurses_lib::StringFace::Normal));
    }
    menu_items->push_back(ncurses_lib::MenuItem
                         (duration_formatter.to_shortstring(total),
                          duration_formatter.to_string(total),
                          ncurses_lib::StringFace::Normal));
    // Per-project totals
    for (auto const &proj : project_totals) {
      menu_items->push_back(ncurses_lib::MenuItem
                           (proj.project_name, proj.project_name,
                            ncurses_lib::StringFace::Bold));
      for (auto const &daily_dur : proj.daily_totals) {
        menu_items->push_back(
            ncurses_lib::MenuItem(duration_formatter.to_shortstring(daily_dur),
                                  duration_formatter.to_string(daily_dur),
                                  ncurses_lib::StringFace::Bold));
      }
      menu_items->push_back(
          ncurses_lib::MenuItem(duration_formatter.to_shortstring(proj.total),
                                duration_formatter.to_string(proj.total),
                                ncurses_lib::StringFace::Bold));
      // Per-task totals
      for (auto const &task : proj.task_totals) {
        menu_items->push_back(ncurses_lib::MenuItem(
            task.task_name, task.task_name, ncurses_lib::StringFace::Normal));
        for (auto const &daily_dur : task.daily_totals) {
          menu_items->push_back(ncurses_lib::MenuItem(
              duration_formatter.to_shortstring(daily_dur),
              duration_formatter.to_string(daily_dur),
              ncurses_lib::StringFace::Normal));
        }
        menu_items->push_back(
            ncurses_lib::MenuItem(duration_formatter.to_shortstring(task.total),
                                  duration_formatter.to_string(task.total),
                                  ncurses_lib::StringFace::Normal));
      }
    }
    return menu_items;
  }
}
