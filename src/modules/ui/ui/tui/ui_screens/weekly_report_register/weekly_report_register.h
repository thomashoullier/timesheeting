/** @file
 *  @brief WeeklyReportRegister definition. */
#ifndef WEEKLY_REPORT_REGISTER_H
#define WEEKLY_REPORT_REGISTER_H

#include "ncurses_lib/menu_ncurses.h"
#include "../ui_component.h"
#include "core/weekly_totals.h"

namespace tui {
  /** @brief Register for displaying a weekly report of durations worked. */
  class WeeklyReportRegister final : public ncurses_lib::MenuNCurses,
                                     public UIComponent {
  public:
    /** @brief Constructor*/
    explicit WeeklyReportRegister(const core::WeeklyTotals &totals);
    /** @brief Replace the totals currently displayed. */
    void set_items(const core::WeeklyTotals &totals);
    config::NormalActions input_loop() final;
    void refresh() final;
    void clear() final;
    void resize() final;
    void update() final;
    void update_status() final;
  };
} // namespace tui

#endif // WEEKLY_REPORT_REGISTER_H
