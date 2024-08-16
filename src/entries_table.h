#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include "ui_screen.h"
#include "register_ncurses.h"

/** @brief Class for holding the table of entries for a given day. */
class EntriesTable : public UIScreen {
public:
  EntriesTable () : reg() {};

  char input_loop() override {
    while (true) {
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
  RegisterNcurses reg;
};

#endif // ENTRIES_TABLE_H
