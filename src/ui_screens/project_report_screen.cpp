#include "project_report_screen.h"
#include "../db/db_sqlite.h"
#include "../logger/logger.h"

ProjectReportScreen::ProjectReportScreen()
    : period_selector_ui(PeriodSelectorUI()),
      total_bar(db().query_entries_duration(
          period_selector_ui.get_current_date_range())),
      reg(ProjectTotalsRegister(db().report_project_totals(
          period_selector_ui.get_current_date_range()))) {}

char ProjectReportScreen::input_loop() {
  UIComponent *cur_focus{&period_selector_ui};
  cur_focus = &reg;
  while (true) {
    auto ch = cur_focus->input_loop();
    switch (ch) {
    case '\t':
      if (cur_focus == &period_selector_ui)
        cur_focus = &reg;
      else
        cur_focus = &period_selector_ui;
      break;
    case '\n': // Update request is passed
      this->update();
      break;
    default:
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
  logger().log("ProjectReportScreen update.");
  period_selector_ui.update();
  auto cur_range = period_selector_ui.get_current_date_range();
  auto overall_duration = db().query_entries_duration(cur_range);
  total_bar.update(overall_duration);
  reg.set_items(db().report_project_totals(cur_range));
  reg.update();
  needs_update = false;
}
