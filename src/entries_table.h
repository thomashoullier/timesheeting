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
};

#endif // ENTRIES_TABLE_H
