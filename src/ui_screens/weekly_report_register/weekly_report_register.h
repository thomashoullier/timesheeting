#ifndef WEEKLY_REPORT_REGISTER_H
#define WEEKLY_REPORT_REGISTER_H

#include "ncurses_lib/menu_ncurses.h"
#include "../ui_component.h"
#include "../../data_objects/weekly_totals.h"

class WeeklyReportRegister : public ncurses_lib::MenuNCurses,
                             public UIComponent {
public:
  /** @brief Constructor*/
  explicit WeeklyReportRegister(const WeeklyTotals &totals);
  /** @brief Replace the totals currently displayed. */
  void set_items(const WeeklyTotals &totals);
  char input_loop() override;
  void refresh() override;
  void clear() override;
  void update() override;
};

#endif // WEEKLY_REPORT_REGISTER_H
