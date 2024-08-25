#include "menu_ncurses.h"
#include "win_ncurses.h"
#include <curses.h>

MenuNCurses::MenuNCurses(const std::vector<std::string> &items,
                         WindowPosition winpos, WindowFormat winformat,
                         int _ncols)
    : WinNCurses(winpos, winformat),
      ncols(_ncols),
      empty_string("") {
  init_menu(items);
}

MenuNCurses::~MenuNCurses() { destroy_menu(); }

void MenuNCurses::select_down_item() { menu_driver(menu, REQ_DOWN_ITEM); }
void MenuNCurses::select_up_item() { menu_driver(menu, REQ_UP_ITEM); }
void MenuNCurses::select_right_item() { menu_driver(menu, REQ_RIGHT_ITEM); }
void MenuNCurses::select_left_item() { menu_driver(menu, REQ_LEFT_ITEM); }

void MenuNCurses::refresh() {
  post_menu(menu);
  WinNCurses::refresh();
}

void MenuNCurses::clear() {
  unpost_menu(menu);
  WinNCurses::clear();
}

void MenuNCurses::set_items(const std::vector<std::string> &items) {
  destroy_menu();
  init_menu(items);
}

void MenuNCurses::set_border() {
  int width = getmaxx(win);
  wmove(win, 0, 0);
  whline(win, '-', width);
  WinNCurses::refresh();
}

void MenuNCurses::unset_border() {
  wmove(win, 0, 0);
  wclrtoeol(win);
  WinNCurses::refresh();
}

const std::string &MenuNCurses::get_current_item_string() const {
  if (display_strings.empty())
    return empty_string;
  else
    return display_strings.at(get_item_index());
}

int MenuNCurses::get_item_index() const {
  return item_index(current_item(menu));
}

int MenuNCurses::get_row_index() const {
  return get_item_index() / ncols; // integer division
}

int MenuNCurses::get_col_index() const {
  return get_item_index() % ncols;
}

void MenuNCurses::init_menu(const std::vector<std::string> &items) {
  init_items(items);
  init_menu_window();
  set_current_item(menu, menu_items.at(0)); // TODO: needed?
}

void MenuNCurses::init_items(const std::vector<std::string> &items) {
  display_strings.resize(items.size());
  menu_items.resize(items.size() + 1);
  for (std::size_t i = 0; i < items.size(); ++i) {
    display_strings.at(i) = items.at(i); // Copy
    // TODO: crop the strings to some specific maximum length,
    //       otherwise it goes out of the screen.
    menu_items.at(i)= new_item(display_strings.at(i).c_str(), NULL);
  }
  menu_items.back() = NULL;
  menu = new_menu(menu_items.data());
}

void MenuNCurses::init_menu_window() {
  set_menu_win(menu, win);
  int ny, nx;
  getmaxyx(win, ny, nx);
  set_menu_sub(menu, derwin(win, ny-1, nx, 1, 0));
  set_menu_format(menu, ny - 1, ncols);
  set_menu_mark(menu, NULL);
}

void MenuNCurses::destroy_menu() {
  unpost_menu(menu);
  free_menu(menu);
  destroy_window();
  for (auto &it : menu_items) {
    free_item(it);
  }
  menu_items.clear();
  display_strings.clear();
}
