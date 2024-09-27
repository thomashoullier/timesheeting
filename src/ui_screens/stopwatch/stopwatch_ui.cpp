#include "stopwatch_ui.h"
#include "../../db/db_sqlite.h"
#include "../status_bar/status_bar_ncurses.h"
#include "../update_manager.h"

StopwatchUI::StopwatchUI() : stopwatch(db().query_entrystaging()) {};

char StopwatchUI::input_loop() {
  stopwatch.set_border();
  while (true) {
    status().print(stopwatch.get_current_item_string());
    auto ch = stopwatch.get_input();
    switch (ch) {
    case 'h':
      stopwatch.select_left_item();
      break;
    case 'i':
      stopwatch.select_right_item();
      break;
    case 'r':
      try {
        rename_item();
        UpdateManager::get().entries_have_changed();
        update();
      } catch (DateParsingFailure &e) {
        status().print_wait("Failed to parse the date. Do nothing.");
        this->clear();
        this->refresh();
      }
      break;
    case ' ':
      set_current_now();
      update();
      break;
    case '\n':
      {
        db().commit_entrystaging();
        UpdateManager::get().entries_have_changed();
        Date now_start;
        db().edit_entrystaging_start(now_start);
        update();
        // Pass the update along by returning the key above.
        return ch;
      } break;
    default:
      stopwatch.unset_border();
      return ch;
    }
  }
}

void StopwatchUI::refresh() { stopwatch.refresh(); }
void StopwatchUI::clear() { stopwatch.clear(); }
void StopwatchUI::update() {
  EntryStaging entry_staging = db().query_entrystaging();
  stopwatch.set_items(entry_staging);
  stopwatch.refresh();
}

void StopwatchUI::rename_item() {
  auto new_str = status().get_user_string();
  auto field_type = stopwatch.get_field_type();
  switch (field_type) {
  case EntryField::project_name:
    db().edit_entrystaging_project_name(new_str);
    break;
  case EntryField::task_name:
    db().edit_entrystaging_task_name(new_str);
    break;
  case EntryField::start: {
    Date new_start_date(new_str);
    db().edit_entrystaging_start(new_start_date);
  } break;
  case EntryField::stop: {
    Date new_stop_date(new_str);
    db().edit_entrystaging_stop(new_stop_date);
  } break;
  case EntryField::location_name: {
    db().edit_entrystaging_location_name(new_str);
  } break;
  default:
    throw std::logic_error("Don't know what to do for renaming this unknown "
                           "field type");
  }
}

void StopwatchUI::set_current_now() {
  auto field_type = stopwatch.get_field_type();
  switch (field_type) {
  case EntryField::start: {
    Date now_start;
    db().edit_entrystaging_start(now_start);
  } break;
  case EntryField::stop: {
    Date now_stop;
    db().edit_entrystaging_stop(now_stop);
  } break;
  default:
    return;
  }
}
