#include "weekly_report_screen.h"

WeeklyReportScreen::WeeklyReportScreen()
    : date_selector(DateSelectorNcurses(
          DateRange(Date(DatePoint::WeekBegin), Date(DatePoint::WeekEnd)))),
      reg(db().report_weekly_totals(date_selector.current_range().start)) {};

char WeeklyReportScreen::input_loop() {
  while (true) {
    auto ch = reg.input_loop();
    switch (ch) {
    case ',':
      date_selector.select_previous_week();
      update();
      break;
    case '.':
      date_selector.select_next_week();
      update();
      break;
    default:
      return ch;
    }
  }
}

void WeeklyReportScreen::refresh() {
  date_selector.refresh();
  reg.refresh();
}

void WeeklyReportScreen::clear() {
  date_selector.clear();
  reg.clear();
}

void WeeklyReportScreen::update() {
  auto first_week_day = date_selector.current_range().start;
  auto week_report = db().report_weekly_totals(first_week_day);
  reg.set_items(week_report);
  this->refresh();
}
