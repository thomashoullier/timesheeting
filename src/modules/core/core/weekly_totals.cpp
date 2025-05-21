#include "weekly_totals.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include "time_lib/duration_displayer.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace core {
  std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
  WeeklyTotals::to_menu_items() const {
    auto menu_items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
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
    std::transform(daily_totals.begin(), daily_totals.end(),
                   std::back_inserter(*menu_items),
                   duration_to_menu_item);
    menu_items->push_back(duration_to_menu_item(total));
    // Per-project totals
    for (auto const &proj : project_totals) {
      menu_items->push_back(ncurses_lib::MenuItem
                           (proj.project_name, proj.project_name,
                            ncurses_lib::StringFace::Bold));
      std::transform(proj.daily_totals.begin(), proj.daily_totals.end(),
                     std::back_inserter(*menu_items),
                     duration_to_bold_menu_item);
      menu_items->push_back(duration_to_bold_menu_item(proj.total));
      // Per-task totals
      for (auto const &task : proj.task_totals) {
        menu_items->push_back(ncurses_lib::MenuItem(
            task.task_name, task.task_name, ncurses_lib::StringFace::Normal));
        std::transform(task.daily_totals.begin(), task.daily_totals.end(),
                       std::back_inserter(*menu_items),
                       duration_to_menu_item);
        menu_items->push_back(duration_to_menu_item(task.total));
      }
    }
    return menu_items;
  }


  ncurses_lib::MenuItem WeeklyTotals::duration_to_menu_item
  (const time_lib::Duration &dur) {
    return ncurses_lib::MenuItem(
        time_lib::DurationDisplayer::get().to_shortstring(dur),
        time_lib::DurationDisplayer::get().to_string(dur),
        ncurses_lib::StringFace::Normal);
  }

  ncurses_lib::MenuItem
  WeeklyTotals::duration_to_bold_menu_item(const time_lib::Duration &dur) {
    return ncurses_lib::MenuItem(
        time_lib::DurationDisplayer::get().to_shortstring(dur),
        time_lib::DurationDisplayer::get().to_string(dur),
        ncurses_lib::StringFace::Bold);
  }
}
