/** @file
 * @brief StopwatchUI definition. */
#ifndef STOPWATCH_UI_H
#define STOPWATCH_UI_H

#include "../ui_component.h"
#include "stopwatch_ncurses.h"
#include "../../db_interface.h"
#include "../status_bar/status_bar_ncurses.h"
#include <memory>

/** @brief High-level UI for the stopwatch. */
template <typename T_DB,
          typename =
              std::enable_if_t<std::is_base_of<DB_Interface, T_DB>::value>>
class StopwatchUI : public UIComponent {
public:
  /** @brief Constructor. */
  explicit StopwatchUI (std::shared_ptr<T_DB> _db,
                        std::shared_ptr<StatusBarNCurses> _status)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      stopwatch(db->query_entrystaging()),
      status(_status) {};

  char input_loop() override {
    stopwatch.set_border();
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
          rename_item();
          update();
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
        set_current_now();
        update();
        break;
      case '\n':
        try {
          db->commit_entrystaging();
          Date now_start;
          db->edit_entrystaging_start(now_start);
          update();
          // Pass the update along by returning the key above.
          return ch;
        } catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
          this->clear();
          this->refresh();
        }
        break;
      default:
        stopwatch.unset_border();
        return ch;
      }
    }
  };

  void refresh() override { stopwatch.refresh(); };
  void clear() override { stopwatch.clear(); };

  void update() override {
    EntryStaging entry_staging = db->query_entrystaging();
    stopwatch.set_items(entry_staging);
    stopwatch.refresh();
  };

private:
  /** @brief Handle to the DB. */
  std::shared_ptr<DB_Interface> db;
  /** @brief Handle to the low-level stopwatch element. */
  StopwatchNcurses stopwatch;
  /** @brief Handle to the status bar. */
  std::shared_ptr<StatusBarNCurses> status;

  /** @brief Rename a field in the entry in staging. */
  void rename_item() {
    auto new_str = stopwatch.query_current_item_rename();
    auto field_type = stopwatch.get_field_type();
    switch (field_type) {
    case EntryField::project_name:
      db->edit_entrystaging_project_name(new_str);
      break;
    case EntryField::task_name:
      db->edit_entrystaging_task_name(new_str);
      break;
    case EntryField::start: {
      Date new_start_date(new_str);
      db->edit_entrystaging_start(new_start_date);
    } break;
    case EntryField::stop: {
      Date new_stop_date(new_str);
      db->edit_entrystaging_stop(new_stop_date);
    } break;
    default:
      throw std::logic_error("Don't know what to do for renaming this unknown "
                             "field type");
    }
  };

  /** @brief Set the current field, if it is a date, to now(). */
  void set_current_now () {
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
};
#endif // STOPWATCH_UI_H
