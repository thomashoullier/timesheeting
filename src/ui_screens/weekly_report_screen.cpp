#include "weekly_report_screen.h"
#include "../db/db_sqlite.h"
#include "../logger/logger.h"

WeeklyReportScreen::WeeklyReportScreen()
  : week_selector{},
    reg(db().report_weekly_totals(week_selector.current_week())) {};

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
  if (needs_update)
    update();
  week_selector.refresh();
  reg.refresh();
}

void WeeklyReportScreen::clear() {
  week_selector.clear();
  reg.clear();
}

void WeeklyReportScreen::update() {
  logger().log("WeeklyReportScreen update.");
  auto week_report = db().report_weekly_totals(week_selector.current_week());
  reg.set_items(week_report);
  needs_update = false;
  this->refresh();
}
