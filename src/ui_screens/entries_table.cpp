#include "entries_table.h"
#include "../db/db_sqlite.h"
#include "status_bar/status_bar.h"
#include "log_lib/logger.h"
#include "update_manager.h"
#include "ui/keys/bound_keys.h"

EntriesTable::EntriesTable()
  : day_selector(),
    total_bar(db().query_entries_duration(day_selector.current_range())),
    reg(db().query_entries(day_selector.current_range())) {}

char EntriesTable::input_loop() {
  reg.set_border();
  while (true) {
    status().print(reg.get_current_item_string());
    auto ch = reg.get_input();
    auto kb = keys::BoundKeys::get().kb;
    if (kb.down.bound_to(ch)) {
      reg.select_down_item();
    } else if (kb.up.bound_to(ch)) {
      reg.select_up_item();
    } else if (kb.right.bound_to(ch)) {
      reg.select_right_item();
    } else if (kb.left.bound_to(ch)) {
      reg.select_left_item();
    } else if (kb.remove.bound_to(ch)) {
      remove_item();
      UpdateManager::get().entries_have_changed();
    } else if (kb.rename.bound_to(ch)) {
      try {
        rename_item();
        update();
        UpdateManager::get().entries_have_changed();
      } catch (time_lib::DateParsingFailure &e) {
        status().print_wait("Failed to parse the date. Do nothing.");
        this->clear();
        this->refresh();
      }
    } else if (kb.next.bound_to(ch)) {
      day_selector.select_next_day();
      auto log_dates = day_selector.current_range().to_string();
      log_lib::logger().log("Selected day range: " + log_dates.at(0) + " ; " +
                            log_dates.at(1),
                            log_lib::LogLevel::debug);
      update();
      day_selector.refresh();
      // TODO: superfluous update?
      total_bar
        .update(db().query_entries_duration(day_selector.current_range()));
    } else if (kb.previous.bound_to(ch)) {
      day_selector.select_previous_day();
      auto log_dates = day_selector.current_range().to_string();
      log_lib::logger().log("Selected day range: " + log_dates.at(0) + " ; " +
                            log_dates.at(1),
                            log_lib::LogLevel::debug);
      update();
      day_selector.refresh();
    } else {
      reg.unset_border();
      return ch;
    }
  }
}

void EntriesTable::refresh() {
  reg.refresh();
  day_selector.refresh();
  total_bar.refresh();
}

void EntriesTable::clear() {
  day_selector.clear();
  reg.clear();
  total_bar.clear();
}

void EntriesTable::update() {
  auto entry_items = db().query_entries(day_selector.current_range());
  reg.set_items(entry_items);
  reg.refresh();
  total_bar.update(db().query_entries_duration(day_selector.current_range()));
  total_bar.refresh();
}

void EntriesTable::rename_item() {
  // TODO: manage the case where the register is empty.
  auto id = reg.get_current_id();
  auto field_type = reg.get_field_type();
  switch (field_type) {
  case EntryField::project_name: {
    auto projects = db().query_projects();
    auto project_names = generic_item_names(projects);
    auto new_str = status().get_user_string_suggestions(project_names);
    db().edit_entry_project(id, new_str);
    break;
  }
  case EntryField::task_name: {
    auto project_id = db().query_entry_project_id(id);
    auto tasks = db().query_tasks(project_id);
    auto task_names = generic_item_names(tasks);
    auto new_str = status().get_user_string_suggestions(task_names);
    db().edit_entry_task(id, new_str);
  } break;
  case EntryField::start: {
    auto new_str = status().get_user_string();
    time_lib::Date new_start_date(new_str);
    db().edit_entry_start(id, new_start_date);
  } break;
  case EntryField::stop: {
    auto new_str = status().get_user_string();
    time_lib::Date new_stop_date(new_str);
    db().edit_entry_stop(id, new_stop_date);
  } break;
  case EntryField::location_name: {
    auto locations = db().query_locations();
    auto location_names = generic_item_names(locations);
    auto new_str = status().get_user_string_suggestions(location_names);
    db().edit_entry_location(id, new_str);
  } break;
  default:
    throw std::logic_error(
        "Don't know what to do for renaming this unknown field type");
  }
}

void EntriesTable::remove_item() {
  bool user_conf = status().query_confirmation("Remove entry? (Y/N)");
  if (!user_conf) {
    return;
  }
  auto id = reg.get_current_id();
  db().delete_entry(id);
  update();
}
