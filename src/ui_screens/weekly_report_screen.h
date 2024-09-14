#ifndef WEEKLY_REPORT_SCREEN_H
#define WEEKLY_REPORT_SCREEN_H

#include "date_selector/date_selector_ncurses.h"
#include "ui_component.h"
#include "../db_interface.h"
#include "status_bar/status_bar_ncurses.h"
#include "weekly_report_register/weekly_report_register.h"

template <typename T_DB,
          typename = std::enable_if_t<std::is_base_of_v<DB_Interface, T_DB>>>
class WeeklyReportScreen : public UIComponent {
public:
  explicit WeeklyReportScreen(std::shared_ptr<T_DB> _db)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      date_selector(DateSelectorNcurses(DateRange(Date(DatePoint::WeekBegin),
                                                  Date(DatePoint::WeekEnd)))),
      reg(db->report_weekly_totals(date_selector.current_range().start)) {};

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
    auto week_report = db->report_weekly_totals(first_week_day);
    reg.set_items(week_report);
    this->refresh();
  };

private:
  std::shared_ptr<DB_Interface> db;
  /** @brief Handle to the date range selector. */
  DateSelectorNcurses date_selector;
  /** @brief Register displaying the weekly totals*/
  WeeklyReportRegister reg;
};

#endif // WEEKLY_REPORT_SCREEN_H
