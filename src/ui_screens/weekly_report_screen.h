#ifndef WEEKLY_REPORT_SCREEN_H
#define WEEKLY_REPORT_SCREEN_H

#include "date_selector/week_selector.h"
#include "ui_component.h"
#include "../db/db_sqlite.h"
#include "weekly_report_register/weekly_report_register.h"

class WeeklyReportScreen : public UIComponent {
public:
  explicit WeeklyReportScreen();

  char input_loop() override;
  void refresh() override;
  void clear() override;
  void update() override;

private:
  /** @brief Handle to the date range selector. */
  WeekSelector week_selector;
  /** @brief Register displaying the weekly totals*/
  WeeklyReportRegister reg;
};

#endif // WEEKLY_REPORT_SCREEN_H
