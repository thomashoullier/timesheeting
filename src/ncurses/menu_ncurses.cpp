#include "menu_ncurses.h"
#include <algorithm>

MenuNCurses::MenuNCurses(const std::vector<std::string> &items,
                         WindowPosition winpos, WindowFormat winformat,
                         int _ncols)
    : WinNCurses(winpos, winformat),
      ncols(_ncols) {
  init_menu(items);
}

MenuNCurses::~MenuNCurses() { destroy_menu(); }

std::string MenuNCurses::query_current_item_rename() {
  return get_user_string(getcury(win));
}

std::string MenuNCurses::query_new_item_name() {
  return get_user_string(getmaxy(win) - 1);
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

// TODO: return a reference instead of copy here, it's not going to change.
std::string MenuNCurses::get_current_item_string() const {
  if (display_strings.empty())
    return "";
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
  set_menu_sub(menu, derwin(win, ny, nx, 0, 0));
  set_menu_format(menu, ny - 1, ncols);
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

std::string MenuNCurses::get_user_string (int display_line) {
  std::string input_buffer{};
  wmove(win, display_line, 0);
  wclrtoeol(win); // TODO: only clear the current item instead.
  bool user_wants_to_input = true;
  while (user_wants_to_input) {
    auto ch = this->get_input();
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
  return sanitize_input(input_buffer);
}

std::string MenuNCurses::sanitize_input(const std::string &input) const {
    auto s = input;
    // left trim
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         [](unsigned char ch) {
                           return !std::isspace(ch);}));
    // right trim
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) {
                           return !std::isspace(ch);}).base(), s.end());
    return s;
}
