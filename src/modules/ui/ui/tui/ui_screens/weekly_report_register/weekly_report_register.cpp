#include "weekly_report_register.h"
#include "config/key.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include "ui/keys/bound_keys.h"
#include "../status_bar/status_bar.h"

namespace tui {
  WeeklyReportRegister::WeeklyReportRegister(const core::WeeklyTotals &totals)
    : MenuNCurses(totals.to_menu_items(),
                  ncurses_lib::WindowPosition::upper,
                  ncurses_lib::WindowFormat::block,
                  {0, 6, 6, 6, 6, 6, 6, 6, 6}) {}

  void WeeklyReportRegister::set_items(const core::WeeklyTotals &totals) {
    MenuNCurses::set_items(totals.to_menu_items());
  }

  config::NormalActions WeeklyReportRegister::input_loop() {
    this->set_border();
    update_status();
    while (true) {
      auto ch = this->get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = kb.normal_mode.action_requested(ch);
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
      case config::NormalActions::previous:
      case config::NormalActions::next:
      case config::NormalActions::entries_screen:
      case config::NormalActions::projects_screen:
      case config::NormalActions::locations_screen:
      case config::NormalActions::project_report_screen:
      case config::NormalActions::duration_display:
      case config::NormalActions::quit:
        //this->unset_border();
        return action;
        break;
      default: // Do nothing
        break;
      }
    }
  }

  void WeeklyReportRegister::refresh() { MenuNCurses::refresh(); }
  void WeeklyReportRegister::clear() { MenuNCurses::clear(); }
  void WeeklyReportRegister::resize() { MenuNCurses::resize(); }
  void WeeklyReportRegister::update() { WeeklyReportRegister::refresh(); }

  void WeeklyReportRegister::update_status() {
    status().print(this->get_current_item_string());
  }
} // namespace tui
