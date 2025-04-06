/** @file
 *  @brief WeeklyReportScreen definition. */
#ifndef WEEKLY_REPORT_SCREEN_H
#define WEEKLY_REPORT_SCREEN_H

#include "date_selector/week_selector.h"
#include "ui_screen.h"
#include "weekly_report_register/weekly_report_register.h"

namespace tui {
/** @brief Weekly report UI screen. */
class WeeklyReportScreen final : public UIScreen {
public:
  /** @brief Constructor. */
  explicit WeeklyReportScreen();

  config::NormalActions input_loop() final;
  void refresh() final;
  void clear() final;
  void resize() final;
  void update() final;
  void update_status() final;

private:
  /** @brief Handle to the date range selector. */
  WeekSelector week_selector;
  /** @brief Register displaying the weekly totals*/
  WeeklyReportRegister reg;
};
} // namespace tui

#endif // WEEKLY_REPORT_SCREEN_H
