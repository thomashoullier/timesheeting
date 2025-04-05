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
  ProjectTotalsRegister::items_to_menu(
      const std::vector<core::ProjectTotal> &items) {
    std::vector<ncurses_lib::MenuItem> menu_items;
    for (const auto &project : items) {
      auto project_menu_items = project.to_menu_items();
      menu_items.insert(menu_items.end(),
                        project_menu_items.begin(),
                        project_menu_items.end());
    }
    return std::make_shared<std::vector<ncurses_lib::MenuItem>>(menu_items);
  }

  void ProjectTotalsRegister::set_items
  (const std::vector<core::ProjectTotal> &totals) {
    MenuNCurses::set_items(items_to_menu(totals));
  }

  config::NormalActions ProjectTotalsRegister::input_loop() {
    this->set_border();
    update_status();
    while (true) {
      auto ch = this->get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = keys::BoundKeys::get().kb.normal_mode.action_requested(ch);
      switch(action) {
      case config::NormalActions::down:
        if(this->select_down_item() ==
           ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          update_status();
        }
        break;
      case config::NormalActions::up:
        if (this->select_up_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          update_status();
        }
        break;
      case config::NormalActions::right:
        if (this->select_right_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          update_status();
        }
        break;
      case config::NormalActions::left:
        if (this->select_left_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          update_status();
        }
        break;
      case config::NormalActions::subtabs:
      case config::NormalActions::entries_screen:
      case config::NormalActions::projects_screen:
      case config::NormalActions::locations_screen:
      case config::NormalActions::weekly_report_screen:
      case config::NormalActions::duration_display:
      case config::NormalActions::quit:
        this->unset_border();
        return action;
        break;
      default: // Do nothing
        break;
      }
    }
  }

  void ProjectTotalsRegister::refresh() { MenuNCurses::refresh(); }
  void ProjectTotalsRegister::clear() { MenuNCurses::clear(); }
  void ProjectTotalsRegister::resize() { MenuNCurses::resize(); }
  void ProjectTotalsRegister::update() { ProjectTotalsRegister::refresh(); }

  void ProjectTotalsRegister::update_status() {
    status().print(this->get_current_item_string());
  }
} // namespace tui
