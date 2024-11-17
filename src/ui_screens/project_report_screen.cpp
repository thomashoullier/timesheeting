#include "project_report_screen.h"
#include "db/db_sqlite.h"
#include "log_lib/logger.h"
#include "ui/keys/bound_keys.h"

ProjectReportScreen::ProjectReportScreen()
    : period_selector_ui(PeriodSelectorUI()),
      total_bar(db::db().query_entries_duration(
          period_selector_ui.get_current_date_range())),
      reg(ProjectTotalsRegister(db::db().report_project_totals(
          period_selector_ui.get_current_date_range()))) {}

char ProjectReportScreen::input_loop() {
  UIComponent *cur_focus{&period_selector_ui};
  cur_focus = &reg;
  while (true) {
    auto ch = cur_focus->input_loop();
    auto kb = keys::BoundKeys::get().kb;
    if (kb.subtabs.bound_to(ch)) {
      if (cur_focus == &period_selector_ui)
        cur_focus = &reg;
      else
        cur_focus = &period_selector_ui;
    } else if (kb.validate.bound_to(ch)) { // Update request is passed
      this->update();
    } else {
      return ch;
    }
  }
}

void ProjectReportScreen::refresh() {
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

void ProjectReportScreen::update() {
  log_lib::logger().log("ProjectReportScreen update.",
                        log_lib::LogLevel::debug);
  period_selector_ui.update();
  auto cur_range = period_selector_ui.get_current_date_range();
  auto overall_duration = db::db().query_entries_duration(cur_range);
  total_bar.update(overall_duration);
  reg.set_items(db::db().report_project_totals(cur_range));
  reg.update();
  needs_update = false;
}
