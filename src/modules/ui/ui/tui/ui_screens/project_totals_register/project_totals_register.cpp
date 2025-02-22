#include "project_totals_register.h"
#include "config/key.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include "ui/keys/bound_keys.h"
#include "../status_bar/status_bar.h"
#include <memory>

namespace tui {
  ProjectTotalsRegister::ProjectTotalsRegister
  (const std::vector<core::ProjectTotal> &totals)
    : MenuNCurses(items_to_menu(totals), ncurses_lib::WindowPosition::upper,
                  ncurses_lib::WindowFormat::block, {0, 0}) {}

  std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
  ProjectTotalsRegister::items_to_menu
  (const std::vector<core::ProjectTotal> &items) {
    auto menu_items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
    for (const auto &it : items) {
      auto it_strings = it.to_strings();
      menu_items->push_back(ncurses_lib::MenuItem(
          it_strings.at(0), it_strings.at(0), ncurses_lib::StringFace::Normal));
      menu_items->push_back(ncurses_lib::MenuItem(
          it_strings.at(1), it_strings.at(1), ncurses_lib::StringFace::Normal));
    }
    return menu_items;
  }

  void ProjectTotalsRegister::set_items
  (const std::vector<core::ProjectTotal> &totals) {
    MenuNCurses::set_items(items_to_menu(totals));
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
  void ProjectTotalsRegister::resize() { MenuNCurses::resize(); }
  void ProjectTotalsRegister::update() { this->refresh(); }
} // namespace tui
