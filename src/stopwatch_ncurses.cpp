#include "stopwatch_ncurses.h"
#include <ncurses.h>
#include <menu.h>

StopwatchNcurses::StopwatchNcurses() {
  init_menu();
}

StopwatchNcurses::~StopwatchNcurses() { destroy_menu(); }

void StopwatchNcurses::refresh() {
  post_menu(menu);
  wrefresh(win);
}

void StopwatchNcurses::clear() {
  wclear(win);
  unpost_menu(menu);
  wrefresh(win);
}

void StopwatchNcurses::select_next_item() { menu_driver(menu, REQ_NEXT_ITEM); }

void StopwatchNcurses::select_previous_item() {
  menu_driver(menu, REQ_PREV_ITEM);
}

void StopwatchNcurses::init_menu() {
  init_items();
  init_menu_window();
  set_current_item(menu, menu_items.at(0));
}

void StopwatchNcurses::init_items() {
  held_items.push_back(Entry{1, "project1", "task", Date(), Date()});
  held_items.push_back(Entry{1, "project2", "task", Date(), Date()});
  held_items.push_back(Entry{1, "project3", "task", Date(), Date()});
  held_items.push_back(Entry{1, "project4", "task", Date(), Date()});
  menu_items.resize(4);

  menu_items.push_back(new_item(held_items.at(0).project_name.c_str(), NULL));
  menu_items.push_back(new_item(held_items.at(1).project_name.c_str(), NULL));
  menu_items.push_back(new_item(held_items.at(2).project_name.c_str(), NULL));
  menu_items.push_back(new_item(held_items.at(3).project_name.c_str(), NULL));

  menu_items.push_back(NULL);
  menu = new_menu(menu_items.data());
  menu_opts_off(menu, O_SHOWDESC);
}

void StopwatchNcurses::init_menu_window() {
  auto max_y = getmaxy(stdscr);
  win = newwin(3, WIDTH, max_y - 10, 1);
  set_menu_win(menu, win);
  set_menu_sub(menu, derwin(win, 2, WIDTH-2, 1, 1));
  set_menu_format(menu, 1, 4);
}

void StopwatchNcurses::destroy_menu() {
  unpost_menu(menu);
  free_menu(menu);
  delwin(win);
  for (auto &it : menu_items) {
    free_item(it);
  }
  menu_items.clear();
  held_items.clear();
}
