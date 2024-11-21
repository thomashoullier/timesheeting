#include "project_totals_register.h"
#include "ui/keys/bound_keys.h"
#include "../status_bar/status_bar.h"

namespace tui {
  ProjectTotalsRegister::ProjectTotalsRegister
  (const std::vector<core::ProjectTotal> &totals)
    : MenuNCurses(items_to_string(totals), ncurses_lib::WindowPosition::upper,
                  ncurses_lib::WindowFormat::block, 2) {}

  std::vector<std::string> ProjectTotalsRegister::items_to_string
  (const std::vector<core::ProjectTotal> &items) {
    std::vector<std::string> display_strings;
    for (const auto &it : items) {
      auto it_strings = it.to_strings();
      display_strings.insert(display_strings.end(), it_strings.begin(),
                             it_strings.end());
    }
    return display_strings;
  }

  void ProjectTotalsRegister::set_items
  (const std::vector<core::ProjectTotal> &totals) {
    MenuNCurses::set_items(items_to_string(totals));
  }

  char ProjectTotalsRegister::input_loop() {
    this->set_border();
    while (true) {
      status().print(this->get_current_item_string());
      auto ch = this->get_input();
      auto kb = keys::BoundKeys::get().kb;
      if (kb.down.bound_to(ch)) {
        this->select_down_item();
      } else if (kb.up.bound_to(ch)) {
        this->select_up_item();
      } else if (kb.right.bound_to(ch)) {
        this->select_right_item();
      } else if (kb.left.bound_to(ch)) {
        this->select_left_item();
      } else {
        this->unset_border();
        return ch;
      }
    }
  }

  void ProjectTotalsRegister::refresh() { MenuNCurses::refresh(); }
  void ProjectTotalsRegister::clear() { MenuNCurses::clear(); }
  void ProjectTotalsRegister::update() { this->refresh(); }
} // namespace tui
