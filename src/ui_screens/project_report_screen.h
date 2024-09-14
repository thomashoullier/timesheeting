#ifndef PROJECT_REPORT_SCREEN_H
#define PROJECT_REPORT_SCREEN_H

#include "period_selector/period_selector_ui.h"
#include "project_totals_register/project_totals_register.h"
#include "status_bar/status_bar_ncurses.h"
#include "total_bar/total_bar.h"
#include "ui_component.h"
#include "../db/db_sqlite.h"
#include <memory>

class ProjectReportScreen : public UIComponent {
public:
  explicit ProjectReportScreen()
    : period_selector_ui(PeriodSelectorUI()),
      total_bar(db().query_entries_duration
                (period_selector_ui.get_current_date_range())),
      reg(ProjectTotalsRegister
          (db().report_project_totals
           (period_selector_ui.get_current_date_range())))
  {};

  char input_loop() override {
    UIComponent *cur_focus {&period_selector_ui};
    cur_focus = &reg;
    while (true) {
      auto ch = cur_focus->input_loop();
      switch(ch) {
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
  };

  void refresh() override {
    period_selector_ui.refresh();
    total_bar.refresh();
    reg.refresh();
  };
  void clear() override {
    period_selector_ui.clear();
    total_bar.clear();
    reg.clear();
  };

  void update() override {
    period_selector_ui.update();
    auto cur_range = period_selector_ui.get_current_date_range();
    auto overall_duration = db().query_entries_duration(cur_range);
    total_bar.update(overall_duration);
    reg.set_items(db().report_project_totals(cur_range));
    reg.update();
  };

private:
  PeriodSelectorUI period_selector_ui;
  TotalBar total_bar;
  ProjectTotalsRegister reg;
};

#endif // PROJECT_REPORT_SCREEN_H
