#include "weekly_report_screen.h"

WeeklyReportScreen::WeeklyReportScreen()
  : week_selector{},
    reg(db().report_weekly_totals(week_selector.current_range().start)) {};

char WeeklyReportScreen::input_loop() {
  while (true) {
    auto ch = reg.input_loop();
    switch (ch) {
    case ',':
      week_selector.select_previous_week();
      update();
      break;
    case '.':
      week_selector.select_next_week();
      update();
      break;
    default:
      return ch;
    }
  }
}

void WeeklyReportScreen::refresh() {
  week_selector.refresh();
  reg.refresh();
}

void WeeklyReportScreen::clear() {
  week_selector.clear();
  reg.clear();
}

void WeeklyReportScreen::update() {
  auto first_week_day = week_selector.current_range().start;
  auto week_report = db().report_weekly_totals(first_week_day);
  reg.set_items(week_report);
  this->refresh();
}
