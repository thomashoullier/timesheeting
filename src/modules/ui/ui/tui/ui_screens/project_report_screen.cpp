#include "project_report_screen.h"
#include "config/key.h"
#include "db/db_sqlite.h"
#include "log_lib/logger.h"
#include <string>
#include "ui/tui/ui_screens/period_selector/period_selector_ui.h"
#include "ui/tui/ui_screens/project_totals_register/project_totals_register.h"
#include "ui/tui/ui_screens/total_bar/total_bar.h"

namespace tui {
  ProjectReportScreen::ProjectReportScreen()
    : period_selector_ui(PeriodSelectorUI()),
      total_bar(db::db().query_entries_duration
                (period_selector_ui.get_current_date_range())),
      reg(ProjectTotalsRegister
          (db::db().report_project_totals
           (period_selector_ui.get_current_date_range()))),
      cur_focus{&period_selector_ui} {}

  config::NormalActions ProjectReportScreen::input_loop() {
    while (true) {
      auto action = cur_focus->input_loop();
      switch(action) {
      case config::NormalActions::subtabs:
        if (cur_focus == &period_selector_ui)
          cur_focus = &reg;
        else
          cur_focus = &period_selector_ui;
        break;
      case config::NormalActions::rename:
        this->update();
        break;
      default:
        return action;
      }
    }
  }

  void ProjectReportScreen::refresh() {
    if (needs_resize)
      resize();
    if (needs_update)
      update();
    period_selector_ui.refresh();
    total_bar.refresh();
    reg.refresh();
  }

  void ProjectReportScreen::clear() {
    period_selector_ui.clear();
    total_bar.clear();
    reg.clear();
  }

  void ProjectReportScreen::resize() {
    log_lib::logger().log("ProjectReportScreen::resize() called.",
                          log_lib::LogLevel::debug);
    period_selector_ui.resize();
    total_bar.resize();
    reg.resize();
    needs_resize = false;
  }

  void ProjectReportScreen::update() {
    log_lib::logger().log("ProjectReportScreen update.",
                          log_lib::LogLevel::debug);
    period_selector_ui.update();
    auto cur_range = period_selector_ui.get_current_date_range();
    auto overall_duration = db::db().query_entries_duration(cur_range);
    total_bar.update(overall_duration);
    total_bar.refresh();
    reg.set_items(db::db().report_project_totals(cur_range));
    reg.update();
    needs_update = false;
  }

  void ProjectReportScreen::update_status() {
    if (cur_focus == &period_selector_ui)
      period_selector_ui.update_status();
    else
      reg.update_status();
  }
} // namespace tui
