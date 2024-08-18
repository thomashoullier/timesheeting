#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include <memory>
#include "db_interface.h"
#include "status_bar_interface.h"
#include "ui_screen.h"
#include "register_ncurses.h"

/** @brief Class for holding the table of entries for a given day. */
template <typename T_DB, typename T_ST,
          typename = std::enable_if_t<
            std::is_base_of<DB_Interface, T_DB>::value &&
            std::is_base_of<StatusBarInterface, T_ST>::value>>
class EntriesTable : public UIScreen {
public:
  explicit EntriesTable (std::shared_ptr<T_DB> _db,
                         std::shared_ptr<T_ST> _status) :
    db(std::static_pointer_cast<DB_Interface>(_db)),
    status(std::static_pointer_cast<StatusBarInterface>(_status)),
    reg(db->query_entries())
  {};

  char input_loop() override {
    while (true) {
      status->print(reg.get_current_cell_string());
      auto ch = reg.query_input();
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
        db->add_entry(1, Date(), Date());
        break;
      case 'r':
        try {
          rename_item();
          // TODO: update the table.
        } catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      default:
        return ch;
      }
    }
  };

  void refresh () override {
    reg.refresh();
  };

  void clear() override {
    reg.clear();
  };

private:
  std::shared_ptr<DB_Interface> db;
  std::shared_ptr<StatusBarInterface> status;
  RegisterNcurses reg;

  void rename_item() {
    //TODO: manage the case where the register is empty.
    //TODO: manage the type of field to rename: project, task, start, stop
    auto new_start_str = reg.query_current_item_rename();
    auto sanitized_start_str = sanitize_input(new_start_str);

    // Convert to a Date object (and check the format of the string in
    // the date class)
    Date new_start_date(sanitized_start_str);

    // DB call
    // TODO: entry id.
    // TODO: prevent the edit if the start date is now after the stop date.
    //       do this check in the DB and raise a DBLogicExcept ?
    db->edit_entry_start(1, new_start_date);
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
