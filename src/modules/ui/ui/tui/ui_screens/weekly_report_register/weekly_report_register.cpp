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
                  {0, 5, 5, 5, 5, 5, 5, 5, 5}) {}

  void WeeklyReportRegister::set_items(const core::WeeklyTotals &totals) {
    MenuNCurses::set_items(totals.to_menu_items());
  }

  config::NormalActions WeeklyReportRegister::input_loop() {
    this->set_border();
    while (true) {
      status().print(this->get_current_item_string());
      auto ch = this->get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = kb.normal_mode.action_requested(ch);
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

  void WeeklyReportRegister::refresh() { MenuNCurses::refresh(); }
  void WeeklyReportRegister::clear() { MenuNCurses::clear(); }
  void WeeklyReportRegister::resize() { MenuNCurses::resize(); }
  void WeeklyReportRegister::update() { this->refresh(); }
} // namespace tui
