#include "entries_table.h"
#include "config/binding_maps.h"
#include "config/key.h"
#include "core/generic_item.h"
#include "db/db_sqlite.h"
#include "log_lib/logger.h"
#include "ncurses_lib/menu_ncurses.h"
#include "status_bar/status_bar.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "ui/keys/bound_keys.h"
#include "ui/tui/ui_screens/date_selector/day_selector.h"
#include "ui/tui/ui_screens/entry_field.h"
#include "ui/tui/ui_screens/register/register_ncurses.h"
#include "ui/tui/ui_screens/total_bar/total_bar.h"
#include "update_manager.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace tui {
  EntriesTable::EntriesTable()
    : day_selector(std::make_shared<DaySelector>()),
      total_bar(db::db().query_entries_duration(day_selector->current_range())),
      reg(db::db().query_entries(day_selector->current_range())) {}

  config::NormalActions EntriesTable::input_loop() {
    reg.set_border();
    update_status();
    while (true) {
      auto ch = reg.get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = kb.normal_mode.action_requested(ch);
      switch (action) {
      case config::NormalActions::down:
        if (reg.select_down_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed)
          update_status();
        break;
      case config::NormalActions::up:
        if (reg.select_up_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed)
          update_status();
        break;
      case config::NormalActions::right:
        if (reg.select_right_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed)
          update_status();
        break;
      case config::NormalActions::left:
        if (reg.select_left_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed)
          update_status();
        break;
      case config::NormalActions::remove:
        remove_item();
        update_status();
        UpdateManager::get().entries_have_changed();
        break;
      case config::NormalActions::rename:
        try {
          rename_item();
          update();
          UpdateManager::get().entries_have_changed();
        } catch (time_lib::DateParsingFailure &e) {
          status().print_wait("Failed to parse the date. Do nothing.");
          this->clear();
          this->refresh();
        }
        update_status();
        break;
      case config::NormalActions::subtabs:
      case config::NormalActions::next:
      case config::NormalActions::previous:
      case config::NormalActions::projects_screen:
      case config::NormalActions::locations_screen:
      case config::NormalActions::project_report_screen:
      case config::NormalActions::weekly_report_screen:
      case config::NormalActions::duration_display:
      case config::NormalActions::quit:
        reg.unset_border();
        return action;
        break;
      default: // Do nothing
        break;
      }
    }
  }

  void EntriesTable::refresh() {
    reg.refresh();
    day_selector->refresh();
    total_bar.refresh();
  }

  void EntriesTable::clear() {
    day_selector->clear();
    reg.clear();
    total_bar.clear();
  }

  void EntriesTable::resize() {
    day_selector->resize();
    reg.resize();
    total_bar.resize();
  }

  void EntriesTable::update() {
    auto entry_items = db::db().query_entries(day_selector->current_range());
    reg.set_items(entry_items);
    reg.refresh();
    total_bar.update(
        db::db().query_entries_duration(day_selector->current_range()));
    total_bar.refresh();
  }

  void EntriesTable::update_status() {
    status().print(reg.get_current_item_string());
  }

  void EntriesTable::select_next_day() {
    day_selector->select_next_day();
    auto log_dates = day_selector->current_range().to_string();
    log_lib::logger().log("Selected day range: " + log_dates.at(0) + " ; " +
                          log_dates.at(1),
                          log_lib::LogLevel::debug);
    update();
    status().print(reg.get_current_item_string());
  }

  void EntriesTable::select_previous_day() {
    day_selector->select_previous_day();
    auto log_dates = day_selector->current_range().to_string();
    log_lib::logger().log("Selected day range: " + log_dates.at(0) + " ; " +
                          log_dates.at(1),
                          log_lib::LogLevel::debug);
    update();
    status().print(reg.get_current_item_string());
  }

  void EntriesTable::rename_item() {
    // TODO: manage the case where the register is empty.
    auto id = reg.get_current_id();
    auto field_type = reg.get_field_type();
    switch (field_type) {
    case EntryField::project_name: {
      auto projects = db::db().query_projects();
      auto project_names = generic_item_names(projects);
      auto new_str = status().get_user_string_suggestions(project_names);
      db::db().edit_entry_project(id, new_str);
      break;
    }
    case EntryField::task_name: {
      auto project_id = db::db().query_entry_project_id(id);
      auto tasks = db::db().query_tasks(project_id);
      auto task_names = generic_item_names(tasks);
      auto new_str = status().get_user_string_suggestions(task_names);
      db::db().edit_entry_task(id, new_str);
    } break;
    case EntryField::start: {
      auto new_str = status().get_user_string();
      time_lib::Date new_start_date(new_str);
      db::db().edit_entry_start(id, new_start_date);
    } break;
    case EntryField::stop: {
      auto new_str = status().get_user_string();
      time_lib::Date new_stop_date(new_str);
      db::db().edit_entry_stop(id, new_stop_date);
    } break;
    case EntryField::location_name: {
      auto locations = db::db().query_locations();
      auto location_names = generic_item_names(locations);
      auto new_str = status().get_user_string_suggestions(location_names);
      db::db().edit_entry_location(id, new_str);
    } break;
    default:
      throw std::logic_error
        ("Don't know what to do for renaming this unknown field type");
    }
  }

  void EntriesTable::remove_item() {
    bool user_conf = status().query_confirmation("Remove entry? (Y/N)");
    if (!user_conf) {
      return;
    }
    auto id = reg.get_current_id();
    db::db().delete_entry(id);
    update();
  }
}
