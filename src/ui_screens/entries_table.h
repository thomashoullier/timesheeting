/** @file
 * @brief EntriesTable definition. */
#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include <memory>
#include <stdexcept>
#include "../data_objects/data_objects.h"
#include "../data_objects/date.h"
#include "date_selector/date_selector_ncurses.h"
#include "../db_interface.h"
#include "status_bar/status_bar_ncurses.h"
#include "register/register_ncurses.h"
#include "ui_component.h"

/** @brief Class for holding the table of entries for a given day. */
template <typename T_DB,
          typename = std::enable_if_t<
            std::is_base_of<DB_Interface, T_DB>::value>>
class EntriesTable : public UIComponent {
public:
  /** @brief Constructor. */
  explicit EntriesTable(std::shared_ptr<T_DB> _db,
                        std::shared_ptr<StatusBarNCurses> _status)
      : db(std::static_pointer_cast<DB_Interface>(_db)),
        status(_status),
        date_selector(),
        reg(db->query_entries(date_selector.current_range())) {};

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
      case 'x':
        remove_item();
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
      case '.':
        date_selector.select_next_day();
        update();
        date_selector.refresh();
        break;
      case ',':
        date_selector.select_previous_day();
        update();
        date_selector.refresh();
        break;
      default:
        return ch;
      }
    }
  };

  void refresh () override {
    reg.refresh();
    date_selector.refresh();
  };

  void clear() override {
    date_selector.clear();
    reg.clear();
  };

  void update() override {
    auto entry_items = db->query_entries(date_selector.current_range());
    reg.set_items(entry_items);
    reg.refresh();
  };

private:
  /** @brief Handle to the DB. */
  std::shared_ptr<DB_Interface> db;
  /** @brief Handle to the status bar. */
  std::shared_ptr<StatusBarNCurses> status;
  /** @brief Handle to the date range selector. */
  DateSelectorNcurses date_selector;
  /** @brief Handle to the register of entries. */
  RegisterNcurses reg;

  /** @brief Rename an entry in the register. */
  void rename_item() {
    //TODO: manage the case where the register is empty.
    auto id = reg.get_current_id();
    auto new_str = reg.query_current_item_rename();
    auto field_type = reg.get_field_type();
    switch (field_type) {
    case EntryField::project_name: {
      db->edit_entry_project(id, new_str);
      break;
    }
    case EntryField::task_name: {
      db->edit_entry_task(id, new_str);
    } break;
    case EntryField::start: {
      Date new_start_date(new_str);
      db->edit_entry_start(id, new_start_date);
    } break;
    case EntryField::stop: {
      Date new_stop_date(new_str);
      db->edit_entry_stop(id, new_stop_date);
    } break;
    default:
      throw std::logic_error(
          "Don't know what to do for renaming this unknown field type");
    }
  };

  /** @brief Remove an entry in the register. */
  void remove_item() {
    bool user_conf = status->query_confirmation("Remove entry? (Y/N)");
    if (!user_conf) {
      return;
    }
    auto id = reg.get_current_id();
    db->delete_entry(id);
    update();
  };
};

#endif // ENTRIES_TABLE_H
