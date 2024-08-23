#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include <memory>
#include <stdexcept>
#include "data_objects.h"
#include "date.h"
#include "date_selector_interface.h"
#include "date_selector_ncurses.h"
#include "db_interface.h"
#include "status_bar_interface.h"
#include "stopwatch_ncurses.h"
#include "ui_screen.h"
#include "register_ncurses.h"

/** @brief Class for holding the table of entries for a given day. */
template <typename T_DB, typename T_ST,
          typename = std::enable_if_t<
            std::is_base_of<DB_Interface, T_DB>::value &&
            std::is_base_of<StatusBarInterface, T_ST>::value>>
class EntriesTable : public UIScreen {
public:
  explicit EntriesTable(std::shared_ptr<T_DB> _db,
                        std::shared_ptr<T_ST> _status)
      : db(std::static_pointer_cast<DB_Interface>(_db)),
        status(std::static_pointer_cast<StatusBarInterface>(_status)),
        date_selector(std::make_unique<DateSelectorNcurses>()),
        reg(db->query_entries(date_selector->current_range())),
        stopwatch(db->query_entrystaging()) {};

  char input_loop() override {
    while (true) {
      status->print(reg.get_current_item_string());
      auto ch = reg.get_input();
      switch (ch) {
      case 'n':
        reg.select_down_item();
        break;
      case 'e':
        reg.select_up_item();
        break;
      case 'i':
        reg.select_right_item();
        break;
      case 'h':
        reg.select_left_item();
        break;
      case 'a': // TODO: put this on the stopwatch instead.
        db->add_entry(1, Date(DatePoint::DayBegin), Date(DatePoint::DayEnd));
        update_register();
        break;
      case 'x':
        remove_item();
        break;
      case 'r':
        try {
          rename_item();
          update_register();
        } catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
          this->clear();
          this->refresh();
        } catch (DateParsingFailure &e) {
          status->print_wait("Failed to parse the date. Do nothing.");
          this->clear();
          this->refresh();
        }
        break;
      case '.':
        date_selector->select_next_day();
        update_register();
        date_selector->refresh();
        break;
      case ',':
        date_selector->select_previous_day();
        update_register();
        date_selector->refresh();
        break;
      case '\t':
        stopwatch_input_loop();
        break;
      default:
        return ch;
      }
    }
  };

  char stopwatch_input_loop() {
    while (true) {
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
          stopwatch_rename_item();
          update_stopwatch();
        } catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
          this->clear();
          this->refresh();
        } catch (DateParsingFailure &e) {
          status->print_wait("Failed to parse the date. Do nothing.");
          this->clear();
          this->refresh();
        }
        break;
      case ' ':
        stopwatch_set_current_now();
        update_stopwatch();
        break;
      case '\n':
        try {
          db->commit_entrystaging();
          Date now_start;
          db->edit_entrystaging_start(now_start);
          update_register();
          update_stopwatch();
        } catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
          this->clear();
          this->refresh();
        }
        break;
      default:
        return ch;
      }
  }
  };

  void refresh () override {
    reg.refresh();
    date_selector->refresh();
    stopwatch.refresh();
  };

  void clear() override {
    stopwatch.clear();
    date_selector->clear();
    reg.clear();
  };

private:
  std::shared_ptr<DB_Interface> db;
  std::shared_ptr<StatusBarInterface> status;
  std::unique_ptr<DateSelectorInterface> date_selector;
  RegisterNcurses reg;
  StopwatchNcurses stopwatch;

  void update_register() {
    auto entry_items = db->query_entries(date_selector->current_range());
    reg.set_items(entry_items);
    reg.refresh();
  };

  void update_stopwatch() {
    EntryStaging entry_staging = db->query_entrystaging();
    stopwatch.set_items(entry_staging);
    stopwatch.refresh();
  };

  void rename_item() {
    //TODO: manage the case where the register is empty.
    auto id = reg.get_current_id();
    auto new_str = reg.query_current_item_rename();
    auto sanitized_str = sanitize_input(new_str);
    auto field_type = reg.get_field_type();
    switch (field_type) {
    case EntryField::project_name: {
      db->edit_entry_project(id, sanitized_str);
      break;
    }
    case EntryField::task_name: {
      db->edit_entry_task(id, sanitized_str);
    } break;
    case EntryField::start: {
      Date new_start_date(sanitized_str);
      db->edit_entry_start(id, new_start_date);
    } break;
    case EntryField::stop: {
      Date new_stop_date(sanitized_str);
      db->edit_entry_stop(id, new_stop_date);
    } break;
    default:
      throw std::logic_error(
          "Don't know what to do for renaming this unknown field type");
    }
  };

  void stopwatch_rename_item() {
    auto new_str = stopwatch.query_current_item_rename();
    auto sanitized_str = sanitize_input(new_str);
    auto field_type = stopwatch.get_field_type();
    switch (field_type) {
    case EntryField::project_name:
      db->edit_entrystaging_project_name(sanitized_str);
      break;
    case EntryField::task_name:
      db->edit_entrystaging_task_name(sanitized_str);
      break;
    case EntryField::start: {
      Date new_start_date(sanitized_str);
      db->edit_entrystaging_start(new_start_date);
    } break;
    case EntryField::stop: {
      Date new_stop_date(sanitized_str);
      db->edit_entrystaging_stop(new_stop_date);
    } break;
    default:
      throw std::logic_error("Don't know what to do for renaming this unknown "
                             "field type");
    }
  };

  /** Set the current item, if it is a date, to now(). */
  void stopwatch_set_current_now () {
    auto field_type = stopwatch.get_field_type();
    switch(field_type) {
    case EntryField::start: {
      Date now_start;
      db->edit_entrystaging_start(now_start);
    } break;
    case EntryField::stop: {
      Date now_stop;
      db->edit_entrystaging_stop(now_stop);
    } break;
    default:
      return;
    }
  };

  void remove_item() {
    bool user_conf = status->query_confirmation("Remove entry? (Y/N)");
    if (!user_conf) {
      return;
    }
    auto id = reg.get_current_id();
    db->delete_entry(id);
    update_register();
  };

  std::string sanitize_input(std::string input) {
    auto s = input;
    // left trim
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         [](unsigned char ch) {
                           return !std::isspace(ch);}));
    // right trim
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) {
                           return !std::isspace(ch);}).base(), s.end());
    return s;
  };
};

#endif // ENTRIES_TABLE_H
