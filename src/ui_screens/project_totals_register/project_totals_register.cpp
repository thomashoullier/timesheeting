#include "project_totals_register.h"

ProjectTotalsRegister::ProjectTotalsRegister
    (const std::vector<ProjectTotal> &totals)
    : MenuNCurses(items_to_string(totals), WindowPosition::upper,
                  WindowFormat::block, 2) {}

std::vector<std::string>
ProjectTotalsRegister::items_to_string(const std::vector<ProjectTotal> &items) {
  std::vector<std::string> display_strings;
  for (const auto &it : items) {
    auto it_strings = it.to_strings();
    display_strings.insert(display_strings.end(), it_strings.begin(),
                           it_strings.end());
  }
  return display_strings;
}

void ProjectTotalsRegister::set_items(const std::vector<ProjectTotal> &totals) {
  MenuNCurses::set_items(items_to_string(totals));
}

char ProjectTotalsRegister::input_loop() {
  this->set_border();
  while (true) {
    status().print(this->get_current_item_string());
    auto ch = this->get_input();
    switch (ch) {
    case 'n':
      this->select_down_item();
      break;
    case 'e':
      this->select_up_item();
      break;
    case 'i':
      this->select_right_item();
      break;
    case 'h':
      this->select_left_item();
      break;
    default:
      this->unset_border();
      return ch;
    }
  }
}

void ProjectTotalsRegister::refresh() { MenuNCurses::refresh(); }
void ProjectTotalsRegister::clear() { MenuNCurses::clear(); }
void ProjectTotalsRegister::update() { this->refresh(); }
