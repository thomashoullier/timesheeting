#ifndef REGISTER_NCURSES_H
#define REGISTER_NCURSES_H

#include <cstddef>
#include <ncurses.h>
#include <menu.h>
#include <vector>
#include "data_objects.h"

// TODO: Factorize with column. Manage any number of columns.
/** @brief Entry register implementation in ncurses.

 A register is a menu with row item made of several columns. */
class RegisterNcurses {
public:
  RegisterNcurses() { init_menu(); };

  ~RegisterNcurses() { destroy_menu(); };

  void refresh() {
    post_menu(menu);
    wrefresh(win);
  };

  void clear() {
    wclear(win);
    unpost_menu(menu);
    wrefresh(win);
  };

  void select_down_item() {menu_driver(menu, REQ_DOWN_ITEM); };
  void select_up_item() {menu_driver(menu, REQ_UP_ITEM); };
  void select_right_item() {menu_driver(menu, REQ_RIGHT_ITEM); };
  void select_left_item() {menu_driver(menu, REQ_LEFT_ITEM); };

  char query_input() { return wgetch(win); }

private:
  static constexpr std::size_t PAGE_LINES{35};
  static constexpr int WIDTH{80};
  WINDOW *win;
  MENU *menu;
  std::vector<Entry> held_items;
  std::vector<ITEM *> menu_items;

  void init_menu() {
    init_items();
    init_menu_window();
    set_current_item(menu, menu_items.at(0));
  };

  void init_items() {
    Date tp;
    held_items.push_back(Entry{10, "Project1", "Task1", tp, tp});
    held_items.push_back(Entry{10, "Project2", "Task2", tp, tp});
    held_items.push_back(Entry{10, "Project3", "Task3", tp, tp});
    for (std::size_t i = 0; i < 3; ++i) {
      // TODO: copy argument items into held items
      // TODO: crop the strings to some specific maximum length,
      //       otherwise it goes out of the screen.
      menu_items.push_back(
          new_item(held_items.at(i).project_name.c_str(), NULL));
      menu_items.push_back(
          new_item(held_items.at(i).task_name.c_str(), NULL));
      menu_items.push_back(new_item(held_items.at(i).start.str.c_str(), NULL));
      menu_items.push_back(new_item(held_items.at(i).stop.str.c_str(), NULL));
    }
    menu_items.push_back(NULL);
    menu = new_menu(menu_items.data());
    menu_opts_off(menu, O_SHOWDESC);
  };

  void init_menu_window() {
    win = newwin(PAGE_LINES + 1, WIDTH, 1, 1);
    set_menu_win(menu, win);
    set_menu_sub(menu, derwin(win, PAGE_LINES, WIDTH - 1, 1, 1));
    set_menu_format(menu, PAGE_LINES - 1, 4);
    //box(win, 0, 0);
  };

  void destroy_menu() {
    unpost_menu(menu);
    free_menu(menu);
    delwin(win);
    for (auto &it : menu_items) {
      free_item(it);
    }
    menu_items.clear();
    held_items.clear();
  };
};

#endif // REGISTER_NCURSES_H
