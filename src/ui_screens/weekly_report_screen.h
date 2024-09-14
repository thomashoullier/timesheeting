#ifndef WEEKLY_REPORT_SCREEN_H
#define WEEKLY_REPORT_SCREEN_H

#include "date_selector/date_selector_ncurses.h"
#include "ui_component.h"
#include "../db/db_sqlite.h"
#include "status_bar/status_bar_ncurses.h"
#include "weekly_report_register/weekly_report_register.h"

class WeeklyReportScreen : public UIComponent {
public:
  explicit WeeklyReportScreen()
    : date_selector(DateSelectorNcurses(DateRange(Date(DatePoint::WeekBegin),
                                                  Date(DatePoint::WeekEnd)))),
      reg(db().report_weekly_totals(date_selector.current_range().start)) {};

  char input_loop() override {
    while(true) {
      auto ch = reg.input_loop();
      switch(ch) {
      case ',':
        date_selector.select_previous_week();
        update();
        break;
      case '.':
        date_selector.select_next_week();
        update();
        break;
      default:
        return ch;
      }
    }
  };

  void refresh() override {
    date_selector.refresh();
    reg.refresh();
  };

  void clear() override {
    date_selector.clear();
    reg.clear();
  };

  void update() override {
    auto first_week_day = date_selector.current_range().start;
    auto week_report = db().report_weekly_totals(first_week_day);
    reg.set_items(week_report);
    this->refresh();
  };

private:
  /** @brief Handle to the date range selector. */
  DateSelectorNcurses date_selector;
  /** @brief Register displaying the weekly totals*/
  WeeklyReportRegister reg;
};

#endif // WEEKLY_REPORT_SCREEN_H
