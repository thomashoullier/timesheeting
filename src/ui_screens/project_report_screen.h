#ifndef PROJECT_REPORT_SCREEN_H
#define PROJECT_REPORT_SCREEN_H

#include "period_selector/period_selector_ui.h"
#include "project_totals_register/project_totals_register.h"
#include "total_bar/total_bar.h"
#include "ui_screen.h"

class ProjectReportScreen : public UIScreen {
public:
  explicit ProjectReportScreen();

  char input_loop() override;
  void refresh() override;
  void clear() override;
  void update() override;

private:
  PeriodSelectorUI period_selector_ui;
  TotalBar total_bar;
  ProjectTotalsRegister reg;
};

#endif // PROJECT_REPORT_SCREEN_H
