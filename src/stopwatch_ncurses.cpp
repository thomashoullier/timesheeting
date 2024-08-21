#include "stopwatch_ncurses.h"
#include "data_objects.h"
#include <menu.h>
#include <ncurses.h>
#include <optional>

StopwatchNcurses::StopwatchNcurses(const EntryStaging &_entry_staging)
    : entry_staging(_entry_staging) {
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

char StopwatchNcurses::query_input() { return wgetch(win); }

std::string StopwatchNcurses::query_current_item_rename() {
  std::string input_buffer{};
  // Clear the display line.
  wclrtoeol(win);
  bool user_wants_to_input = true;
  while (user_wants_to_input) { // Item input loop.
    auto ch = wgetch(win);
    switch (ch) {
    case '\n': // User validates the input.
      user_wants_to_input = false;
      break;
    case 27: // User wants to cancel.
      input_buffer.clear();
      user_wants_to_input = false;
      break;
    case 127: // Erase last character
      if (!input_buffer.empty()) {
        input_buffer.pop_back();
        // Remove character from screen
        int y, x;
        getyx(win, y, x);
        auto xmax = getmaxx(win);
        if (x == 0) { // jump to the end of the previous line.
          wmove(win, y - 1, xmax - 1);
        } else {
          wmove(win, y, x - 1);
        }
        wdelch(win);
      }
      break;
    default: // Gets added to item.
      input_buffer.push_back(ch);
      waddch(win, ch);
      break;
    }
  }
  return input_buffer;
}

EntryField StopwatchNcurses::get_field_type() {
  auto cell_index = item_index(current_item(menu));
  return EntryField(cell_index);
}

void StopwatchNcurses::set_items(const EntryStaging &item) {
  destroy_menu();
  entry_staging = item;
  init_menu();
}

void StopwatchNcurses::init_menu() {
  init_items();
  init_menu_window();
  set_current_item(menu, menu_items.at(0));
}

void StopwatchNcurses::init_items() {
  menu_string.at(0) = entry_staging.project_name.value_or(" ");
  menu_string.at(1) = entry_staging.task_name.value_or(" ");
  if (entry_staging.start.has_value())
    menu_string.at(2) = entry_staging.start.value().str;
  else
    menu_string.at(2) = " ";
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
