#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include "ui_screen.h"
#include <cstddef>
#include <ncurses.h>
#include <menu.h>
#include <vector>

// TODO: make a generic interface to a register-like thing and a
//       ncurses implementation of it. And make this class into
//       a template.
// TODO: Factorize the menu from here with the one in columns.
//       Maybe factorize with column altogether.
/** @brief Class for holding the table of entries for a given day. */
class EntriesTable : public UIScreen {
public:
  EntriesTable () {
    init_menu();
  };

  ~EntriesTable () {
    destroy_menu();
  };

  char input_loop() override {
    return wgetch(win);
  };

  void refresh () override {
    post_menu(menu);
    wrefresh(win);
  };

  void clear() override {
    wclear(win);
    unpost_menu(menu);
    wrefresh(win);
  };

private:
  static constexpr std::size_t PAGE_LINES{35};
  static constexpr int WIDTH{80};
  WINDOW *win;
  MENU *menu;
  std::vector<ITEM *> menu_items;

  void init_menu() {
    init_items();
    init_menu_window();
    set_current_item(menu, menu_items.at(0));
    refresh();
  };

  void init_items() {
    menu_items.push_back(new_item("Project1", NULL));
    menu_items.push_back(new_item("Task1", NULL));
    menu_items.push_back(new_item("Start1", NULL));
    menu_items.push_back(new_item("Stop1", NULL));
    menu_items.push_back(NULL);
    menu = new_menu(menu_items.data());
  };

  void init_menu_window() {
    win = newwin(PAGE_LINES + 1, WIDTH, 1, 1);
    set_menu_win(menu, win);
    set_menu_sub(menu, derwin(win, PAGE_LINES, WIDTH - 2, 1, 1));
    set_menu_format(menu, PAGE_LINES - 1, 4);
    box(win, 0, 0);
  };

  void destroy_menu() {
    unpost_menu(menu);
    free_menu(menu);
    delwin(win);
    for (auto &it : menu_items) {
      free_item(it);
    }
    menu_items.clear();
  };
};

#endif // ENTRIES_TABLE_H
