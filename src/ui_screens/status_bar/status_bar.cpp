#include "status_bar.h"
#include <algorithm>

StatusBarNCurses &StatusBarNCurses::get() {
  static StatusBarNCurses instance;
  return instance;
}

StatusBarNCurses::StatusBarNCurses()
    : BarNCurses(WindowPosition::bottom, WindowFormat::line) {}

void StatusBarNCurses::print(const std::string &msg) { BarNCurses::print(msg); }

void StatusBarNCurses::print_wait(const std::string &msg) {
  this->print(msg);
  this->get_input();
}

bool StatusBarNCurses::query_confirmation(const std::string &msg) {
  this->print(msg);
  auto ch = this->get_input();
  if (ch == 'y') {
    return true;
  } else {
    return false;
  }
}

StatusBarNCurses &status() { return StatusBarNCurses::get(); }

std::string StatusBarNCurses::get_user_string() {
  // TODO: decouple from ncurses functions.
  //       Write these functions as an interface to BarNCurses.
  std::string input_buffer{};
  wmove(win, 0, 0);
  wclrtoeol(win);
  curs_set(1);
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
  curs_set(0);
  return sanitize_input(input_buffer);
}

std::string StatusBarNCurses::sanitize_input(const std::string &input) const {
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
