#include "weekly_report_register.h"

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

void WeeklyReportRegister::refresh() { MenuNCurses::refresh(); }
void WeeklyReportRegister::clear() { MenuNCurses::clear(); }
void WeeklyReportRegister::update() { this->refresh(); }
