#include "weekly_report_screen.h"
#include "db/db_sqlite.h"
#include "log_lib/logger.h"
#include "ui/keys/bound_keys.h"

namespace tui {
  WeeklyReportScreen::WeeklyReportScreen()
    : week_selector{},
      reg(db::db().report_weekly_totals(week_selector.current_week())) {};

  int WeeklyReportScreen::input_loop() {
    while (true) {
      auto ch = reg.input_loop();
      auto kb = keys::BoundKeys::get().kb;
      if (kb.navigation.previous.bound_to(ch)) {
        week_selector.select_previous_week();
        update();
      } else if (kb.navigation.next.bound_to(ch)) {
        week_selector.select_next_week();
        update();
      } else {
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
    log_lib::logger().log("WeeklyReportScreen update.",
                          log_lib::LogLevel::debug);
    auto week_report =
      db::db().report_weekly_totals(week_selector.current_week());
    reg.set_items(week_report);
    needs_update = false;
    this->refresh();
  }
} // namespace tui
