#include "weekly_report_register.h"

WeeklyReportRegister::WeeklyReportRegister(
    const WeeklyTotals &totals, std::shared_ptr<StatusBarNCurses> _status)
    : MenuNCurses(totals.to_strings(), WindowPosition::upper,
                  WindowFormat::block, 9),
      status(_status) {}

void WeeklyReportRegister::set_items(const WeeklyTotals &totals) {
  MenuNCurses::set_items(totals.to_strings());
}

char WeeklyReportRegister::input_loop() {
  this->set_border();
  while (true) {
    status->print(this->get_current_item_string());
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
