#include "stopwatch_ncurses.h"
#include "data_objects.h"
#include <ncurses.h>
#include <menu.h>
#include <optional>

StopwatchNcurses::StopwatchNcurses()
  : entry_staging(EntryStaging{"Project", "Task", Date(), std::nullopt}) {
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

char StopwatchNcurses::query_input() {
  return wgetch(win);
}

void StopwatchNcurses::init_menu() {
  init_items();
  init_menu_window();
  set_current_item(menu, menu_items.at(0));
}

void StopwatchNcurses::init_items() {
  menu_string.at(0) = entry_staging.project_name.value_or("");
  menu_string.at(1) = entry_staging.task_name.value_or("");
  menu_string.at(2) = entry_staging.start.str;
  if (entry_staging.stop.has_value())
    menu_string.at(3) = entry_staging.stop.value().str;
  else
    menu_string.at(3) = " ";

  menu_items.at(0) = new_item(menu_string.at(0).c_str(), NULL);
  menu_items.at(1) = new_item(menu_string.at(1).c_str(), NULL);
  menu_items.at(2) = new_item(menu_string.at(2).c_str(), NULL);
  menu_items.at(3) = new_item(menu_string.at(3).c_str(), NULL);

  menu_items.at(4) = NULL;
  menu = new_menu(menu_items.data());
  menu_opts_off(menu, O_SHOWDESC);
}

void StopwatchNcurses::init_menu_window() {
  auto max_y = getmaxy(stdscr);
  win = newwin(1, WIDTH, max_y - 3, 1);
  set_menu_win(menu, win);
  set_menu_sub(menu, derwin(win, 1, WIDTH - 2, 0, 1));
  set_menu_format(menu, 1, 4);
}

void StopwatchNcurses::destroy_menu() {
  unpost_menu(menu);
  free_menu(menu);
  delwin(win);
  for (auto &it : menu_items) {
    free_item(it);
  }
}
