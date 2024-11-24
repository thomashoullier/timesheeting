#include "weekly_report_register.h"
#include "ui/keys/bound_keys.h"
#include "../status_bar/status_bar.h"

namespace tui {
  WeeklyReportRegister::WeeklyReportRegister(const core::WeeklyTotals &totals)
    : MenuNCurses(totals.to_strings(), totals.to_shortstrings(),
                  ncurses_lib::WindowPosition::upper,
                  ncurses_lib::WindowFormat::block, 9) {}

  void WeeklyReportRegister::set_items(const core::WeeklyTotals &totals) {
    MenuNCurses::set_items(totals.to_strings(), totals.to_shortstrings());
  }

  int WeeklyReportRegister::input_loop() {
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

  void WeeklyReportRegister::refresh() { MenuNCurses::refresh(); }
  void WeeklyReportRegister::clear() { MenuNCurses::clear(); }
  void WeeklyReportRegister::update() { this->refresh(); }
} // namespace tui
