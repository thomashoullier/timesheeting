#include "project_total.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include "time_lib/duration_displayer.h"

namespace core {
  std::vector<ncurses_lib::MenuItem>
  ProjectTotal::to_menu_items() const {
    auto menu_items = std::vector<ncurses_lib::MenuItem>();
    menu_items.push_back(ncurses_lib::MenuItem(project_name, project_name,
                                                ncurses_lib::StringFace::Bold));
    menu_items.push_back(ncurses_lib::MenuItem(
        time_lib::DurationDisplayer::get().to_string(total),
        time_lib::DurationDisplayer::get().to_string(total),
        ncurses_lib::StringFace::Bold));
    for (const auto &task_total : task_totals) {
      menu_items.push_back(
          ncurses_lib::MenuItem(task_total.task_name, task_total.task_name,
                                ncurses_lib::StringFace::Normal));
      menu_items.push_back(ncurses_lib::MenuItem(
          time_lib::DurationDisplayer::get().to_string(task_total.total),
          time_lib::DurationDisplayer::get().to_string(task_total.total),
          ncurses_lib::StringFace::Normal));
    }
    return menu_items;
  }
}
