#ifndef REGISTER_NCURSES_H
#define REGISTER_NCURSES_H

#include <cstddef>
#include <ncurses.h>
#include <menu.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <exception>
#include "data_objects.h"

/** @brief Entry field type (as displayed in the menu). */
enum EntryField { project_name = 0, task_name = 1, start = 2, stop = 3 };

// TODO: Factorize with column. Manage any number of columns.
/** @brief Entry register implementation in ncurses.

 A register is a menu with row item made of several columns. */
class RegisterNcurses {
public:
  RegisterNcurses(const std::vector<Entry> &items, int _display_line) :
    display_line(_display_line) {
    init_menu(items);
  };

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

  void set_items(const std::vector<Entry> &items) {
    destroy_menu();
    init_menu(items);
  };

  Id get_current_id() {
    if (held_items.empty()) {
      // TODO: throw a column empty exception.
      return 1;
    }
    auto item_index = get_item_index();
    return held_items.at(item_index).id;
  }

  EntryField get_field_type () {
    int field_index = get_cell_index() % ncols;
    return EntryField(field_index);
  }

  std::string get_current_cell_string () {
    auto cell_index = get_cell_index();
    if (cell_index >= 0) {
      auto it_index = get_item_index();
      auto field_type = get_field_type();
      auto *it = &held_items.at(it_index);
      switch(field_type) {
      case EntryField::project_name:
        return it->project_name;
        break;
      case EntryField::task_name:
        return it->task_name;
        break;
      case EntryField::start:
        return it->start.str;
        break;
      case EntryField::stop:
        return it->stop.str;
        break;
      default:
        throw std::logic_error("Unknown index for an Entry field.");
      }
    } else {
      return "";
    }
  }

  void select_down_item() {menu_driver(menu, REQ_DOWN_ITEM); };
  void select_up_item() {menu_driver(menu, REQ_UP_ITEM); };
  void select_right_item() {menu_driver(menu, REQ_RIGHT_ITEM); };
  void select_left_item() {menu_driver(menu, REQ_LEFT_ITEM); };

  char query_input() { return wgetch(win); }
  std::string query_current_item_rename() {
    return query_row_input();
  };

private:
  int display_line;
  static constexpr std::size_t PAGE_LINES{35};
  static constexpr int WIDTH{80};
  static constexpr int ncols {4};
  WINDOW *win;
  MENU *menu;
  std::vector<Entry> held_items;
  std::vector<ITEM *> menu_items;

  void init_menu(const std::vector<Entry> &items) {
    init_items(items);
    init_menu_window();
    set_current_item(menu, menu_items.at(0));
  };

  void init_items(const std::vector<Entry> &items) {
    held_items.resize(items.size());
    for (std::size_t i = 0; i < items.size(); ++i) {
      held_items.at(i) = items.at(i);
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

  int get_cell_index() {
    auto cell_index = item_index(current_item(menu));
    return cell_index;
  }

  int get_item_index () {
    auto cell_index = get_cell_index();
    int item_index = cell_index / ncols; // integer division
    return item_index;
  }

  void init_menu_window() {
    win = newwin(PAGE_LINES + 1, WIDTH, display_line, 1);
    set_menu_win(menu, win);
    set_menu_sub(menu, derwin(win, PAGE_LINES, WIDTH - 1, 1, 1));
    set_menu_format(menu, PAGE_LINES - 1, ncols);
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

  std::string query_row_input () {
    std::string input_buffer {};
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
            wmove(win, y-1, xmax-1);
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
  };
};

#endif // REGISTER_NCURSES_H
