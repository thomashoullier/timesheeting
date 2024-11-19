#ifndef WEEKLY_REPORT_REGISTER_H
#define WEEKLY_REPORT_REGISTER_H

#include "ncurses_lib/menu_ncurses.h"
#include "../ui_component.h"
#include "core/weekly_totals.h"

namespace tui {
  class WeeklyReportRegister : public ncurses_lib::MenuNCurses,
                               public UIComponent {
  public:
    /** @brief Constructor*/
    explicit WeeklyReportRegister(const core::WeeklyTotals &totals);
    /** @brief Replace the totals currently displayed. */
    void set_items(const core::WeeklyTotals &totals);
    char input_loop() override;
    void refresh() override;
    void clear() override;
    void update() override;
  };
} // namespace tui

#endif // WEEKLY_REPORT_REGISTER_H
