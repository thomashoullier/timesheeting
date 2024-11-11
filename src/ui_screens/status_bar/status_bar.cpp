#include "status_bar.h"
#include "suggestion.h"
#include <algorithm>
#include "../../bound_keys.h"

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
  std::string input_buffer{};
  this->prepare_input();
  this->set_cursor_visibility(true);
  bool user_wants_to_input = true;
  while (user_wants_to_input) {
    auto ch = this->get_input();
    auto kb = BoundKeys::get().kb;
    if (kb.validate.bound_to(ch)) {
      user_wants_to_input = false;
    } else if (kb.cancel.bound_to(ch)) {
      input_buffer.clear();
      user_wants_to_input = false;
    } else if (ch == 127) { // Backspace
      if (!input_buffer.empty()) {
        input_buffer.pop_back();
        this->remove_char();
      }
    } else {
      if (input_buffer.size() < this->max_size() - 1) {
        input_buffer.push_back(ch);
        this->add_char(ch);
      }
    }
  }
  this->set_cursor_visibility(false);
  return sanitize_input(input_buffer);
}

std::string StatusBarNCurses::get_user_string_suggestions
(const std::vector<std::string> &suggestions) {
  std::string input_buffer{};
  this->prepare_input();
  this->set_cursor_visibility(true);
  bool user_wants_to_input = true;
  while (user_wants_to_input) {
    auto ch = this->get_input();
    auto kb = BoundKeys::get().kb;
    if (kb.validate.bound_to(ch)) {
      user_wants_to_input = false;
    } else if (kb.cancel.bound_to(ch)) {
      input_buffer.clear();
      user_wants_to_input = false;
    } else if (ch == 127) { // Backspace
      if (!input_buffer.empty()) {
        input_buffer.pop_back();
        this->remove_char();
      }
    } else {
      if (input_buffer.size() < this->max_size() - 1) {
        input_buffer.push_back(ch);
        this->add_char(ch);
      }
    }
    // Manage the suggestions
    auto best_match = suggestion::best_match(input_buffer, suggestions);
    this->print_after_cursor(best_match);
  }
  this->set_cursor_visibility(false);
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
