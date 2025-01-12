#include "project_totals_register.h"
#include "config/key.h"
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

  config::NormalActions ProjectTotalsRegister::input_loop() {
    this->set_border();
    while (true) {
      status().print(this->get_current_item_string());
      auto ch = this->get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = keys::BoundKeys::get().kb.normal_mode.action_requested(ch);
      switch(action) {
      case config::NormalActions::down:
        this->select_down_item();
        break;
      case config::NormalActions::up:
        this->select_up_item();
        break;
      case config::NormalActions::right:
        this->select_right_item();
        break;
      case config::NormalActions::left:
        this->select_left_item();
        break;
      default:
        this->unset_border();
        return action;
      }
    }
  }

  void ProjectTotalsRegister::refresh() { MenuNCurses::refresh(); }
  void ProjectTotalsRegister::clear() { MenuNCurses::clear(); }
  void ProjectTotalsRegister::update() { this->refresh(); }
} // namespace tui
