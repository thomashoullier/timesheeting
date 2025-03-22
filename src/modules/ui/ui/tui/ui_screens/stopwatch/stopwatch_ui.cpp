#include "stopwatch_ui.h"
#include "config/key.h"
#include "db/db_sqlite.h"
#include "../status_bar/status_bar.h"
#include "../update_manager.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ui/keys/bound_keys.h"

namespace tui {
  StopwatchUI::StopwatchUI() : stopwatch(db::db().query_entrystaging()) {};

  config::NormalActions StopwatchUI::input_loop() {
    stopwatch.set_border();
    status().print(stopwatch.get_current_item_string());
    while (true) {
      auto ch = stopwatch.get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = kb.normal_mode.action_requested(ch);
      switch(action) {
      case config::NormalActions::left:
        if (stopwatch.select_left_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          status().print(stopwatch.get_current_item_string());
        }
        break;
      case config::NormalActions::right:
        if (stopwatch.select_right_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          status().print(stopwatch.get_current_item_string());
        }
        break;
      case config::NormalActions::rename:
        try {
          rename_item();
          UpdateManager::get().entries_have_changed();
          update();
        } catch (time_lib::DateParsingFailure &e) {
          status().print_wait("Failed to parse the date. Do nothing.");
          this->clear();
          this->refresh();
        }
        status().print(stopwatch.get_current_item_string());
        break;
      case config::NormalActions::set_now:
        if (set_current_now()) {
          update();
          status().print(stopwatch.get_current_item_string());
        }
        break;
      case config::NormalActions::commit_entry: 
        if (db::db().commit_entrystaging()) {
          UpdateManager::get().entries_have_changed();
          time_lib::Date now_start;
          db::db().edit_entrystaging_start(now_start);
          update();
          return action; // Pass the update
        }
        break;
      case config::NormalActions::subtabs:
      case config::NormalActions::projects_screen:
      case config::NormalActions::locations_screen:
      case config::NormalActions::project_report_screen:
      case config::NormalActions::weekly_report_screen:
      case config::NormalActions::duration_display:
      case config::NormalActions::quit:
        stopwatch.unset_border();
        return action;
        break;
      default: // Do nothing
        break;
      }
    }
  }

  void StopwatchUI::refresh() { stopwatch.refresh(); }
  void StopwatchUI::clear() { stopwatch.clear(); }
  void StopwatchUI::resize() {
    stopwatch.resize();
  }
  void StopwatchUI::update() {
    core::EntryStaging entry_staging = db::db().query_entrystaging();
    stopwatch.set_items(entry_staging);
    stopwatch.refresh();
  }

  void StopwatchUI::rename_item() {
    auto field_type = stopwatch.get_field_type();
    switch (field_type) {
    case EntryField::project_name: {
      auto projects = db::db().query_projects_active();
      auto project_names = generic_item_names(projects);
      auto new_str = status().get_user_string_suggestions(project_names);
      if (!new_str.empty()) {
        db::db().edit_entrystaging_project_name(new_str);
      }
    } break;
    case EntryField::task_name: {
      auto current_entry_staging = db::db().query_entrystaging();
      if(current_entry_staging.project_name.has_value()) {
        auto project_id = db::db().query_entrystaging_project_id();
        auto tasks = db::db().query_tasks_active(project_id);
        auto task_names = generic_item_names(tasks);
        auto new_str = status().get_user_string_suggestions(task_names);
        if (!new_str.empty()) {
          db::db().edit_entrystaging_task_name(new_str);
        }
      }
    } break;
    case EntryField::start: {
      auto new_str = status().get_user_string();
      if (!new_str.empty()) {
        time_lib::Date new_start_date(new_str);
        db::db().edit_entrystaging_start(new_start_date);
      }
    } break;
    case EntryField::stop: {
      auto new_str = status().get_user_string();
      if (!new_str.empty()) {
        time_lib::Date new_stop_date(new_str);
        db::db().edit_entrystaging_stop(new_stop_date);
      }
    } break;
    case EntryField::location_name: {
      auto locations = db::db().query_locations_active();
      auto location_names = generic_item_names(locations);
      auto new_str = status().get_user_string_suggestions(location_names);
      if (!new_str.empty()) {
        db::db().edit_entrystaging_location_name(new_str);
      }
    } break;
    default:
      throw std::logic_error("Don't know what to do for renaming this unknown "
                             "field type");
    }
  }

  bool StopwatchUI::set_current_now() {
    auto field_type = stopwatch.get_field_type();
    switch (field_type) {
    case EntryField::start: {
      time_lib::Date now_start;
      db::db().edit_entrystaging_start(now_start);
    }
      return true;
    case EntryField::stop: {
      time_lib::Date now_stop;
      db::db().edit_entrystaging_stop(now_stop);
    }
      return true;
    default:
      return false;
    }
  }
} // namespace tui
