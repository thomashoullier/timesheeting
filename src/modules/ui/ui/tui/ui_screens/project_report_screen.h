/** @file
 *  @brief ProjectReportScreen definition. */
#ifndef PROJECT_REPORT_SCREEN_H
#define PROJECT_REPORT_SCREEN_H

#include "config/key.h"
#include "period_selector/period_selector_ui.h"
#include "project_totals_register/project_totals_register.h"
#include "total_bar/total_bar.h"
#include "ui_screen.h"

namespace tui {
  /** @brief The UIScreen presenting the project totals report. */
  class ProjectReportScreen : public UIScreen {
  public:
    /** @brief Constructor. */
    explicit ProjectReportScreen();

    config::NormalActions input_loop() override;
    void refresh() override;
    void clear() override;
    void update() override;

  private:
    /** @brief Selector for the date range to consider. */
    PeriodSelectorUI period_selector_ui;
    /** @brief Bar for displaying the grand total of duration worked. */
    TotalBar total_bar;
    /** @brief Register display of totals per project. */
    ProjectTotalsRegister reg;
  };
} // namespace tui

#endif // PROJECT_REPORT_SCREEN_H
