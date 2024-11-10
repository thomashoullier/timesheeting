#include "weekly_report_register.h"
#include "../../bound_keys.h"

WeeklyReportRegister::WeeklyReportRegister(const WeeklyTotals &totals)
  : MenuNCurses(totals.to_strings(), totals.to_shortstrings(),
                WindowPosition::upper, WindowFormat::block, 9) {}

void WeeklyReportRegister::set_items(const WeeklyTotals &totals) {
  MenuNCurses::set_items(totals.to_strings(), totals.to_shortstrings());
}

char WeeklyReportRegister::input_loop() {
  this->set_border();
  while (true) {
    status().print(this->get_current_item_string());
    auto ch = this->get_input();
    auto kb = BoundKeys::get().kb;
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
