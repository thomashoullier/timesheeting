#include "menu_ncurses.h"
#include "win_ncurses.h"
#include <menu.h>

MenuNCurses::MenuNCurses(const std::vector<std::string> &items,
                         WindowPosition winpos, WindowFormat winformat,
                         int _ncols)
    : WinNCurses(winpos, winformat),
      ncols(_ncols) {
  init_menu(items);
}

MenuNCurses::~MenuNCurses() { destroy_menu(); }

std::string MenuNCurses::query_current_item_rename() {
  std::string input_buffer{};
  wclrtoeol(win); // TODO: only clear the current item instead.
  bool user_wants_to_input = true;
  while (user_wants_to_input) {
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
    menu_items.push_back(new_item(display_strings.at(i).c_str(), NULL));
  }
  menu_items.push_back(NULL);
  menu = new_menu(menu_items.data());
}

void MenuNCurses::init_menu_window() {
  set_menu_win(menu, win);
  int ny, nx;
  getmaxyx(win, ny, nx);
  set_menu_sub(menu, derwin(win, ny, nx, 0, 0));
  set_menu_format(menu, ny - 1, ncols);
}

void MenuNCurses::destroy_menu() {
  unpost_menu(menu);
  free_menu(menu);
  for (auto &it : menu_items) {
    free_item(it);
  }
  menu_items.clear();
  display_strings.clear();
}
