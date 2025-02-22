#include "weekly_report_screen.h"
#include "config/key.h"
#include "db/db_sqlite.h"
#include "log_lib/logger.h"

namespace tui {
  WeeklyReportScreen::WeeklyReportScreen()
    : week_selector{},
      reg(db::db().report_weekly_totals(week_selector.current_week())) {};

  config::NormalActions WeeklyReportScreen::input_loop() {
    while (true) {
      auto action = reg.input_loop();
      switch(action) {
      case config::NormalActions::previous:
        week_selector.select_previous_week();
        update();
        break;
      case config::NormalActions::next:
        week_selector.select_next_week();
        update();
        break;
      default:
        return action;
      }
    }
  }

  void WeeklyReportScreen::refresh() {
    if (needs_resize)
      resize();
    if (needs_update)
      update();
    week_selector.refresh();
    reg.refresh();
  }

  void WeeklyReportScreen::clear() {
    week_selector.clear();
    reg.clear();
  }

  void WeeklyReportScreen::resize() {
    log_lib::logger().log("WeeklyReportScreen::resize() called.",
                          log_lib::LogLevel::debug);
    week_selector.resize();
    reg.resize();
    needs_resize = false;
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
