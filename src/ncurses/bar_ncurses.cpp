#include "bar_ncurses.h"
#include "win_ncurses.h"
#include <stdexcept>
#include <algorithm>

BarNCurses::BarNCurses(WindowPosition winpos, WindowFormat winformat)
    : WinNCurses(winpos, filter_window_format(winformat)) {}

void BarNCurses::print(const std::string &msg) const {
  this->clear();
  wmove(win, 0, 0);
  wprintw(win, msg.c_str());
  this->refresh();
}

void BarNCurses::print_right(const std::string &msg) const {
  this->clear();
  int str_len = msg.length();
  int max_x = getmaxx(win);
  wmove(win, 0, max_x - str_len);
  wprintw(win, msg.c_str());
  this->refresh();
}

WindowFormat BarNCurses::filter_window_format(WindowFormat winformat) {
  switch (winformat) {
  case WindowFormat::line:
    return winformat;
  case WindowFormat::half_line:
    return winformat;
  default:
    throw std::logic_error("WindowFormat is incompatible with BarNCurses.");
  }
}

std::string BarNCurses::get_user_string() {
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

std::string BarNCurses::sanitize_input(const std::string &input) const {
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
