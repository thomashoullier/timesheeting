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
  explicit WeeklyReportScreen(std::shared_ptr<T_DB> _db,
                              std::shared_ptr<StatusBarNCurses> _status)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      status(_status),
      date_selector(DateSelectorNcurses(DateRange(Date(DatePoint::WeekBegin),
                                                  Date(DatePoint::WeekEnd)))),
      reg(db->report_weekly_totals(date_selector.current_range().start),
          _status)
  {};

  char input_loop() override {
    while(true) {
      auto ch = reg.input_loop();
      switch(ch) {
        // TODO: select next/prev week
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
    // TODO
    this->refresh();
  };

private:
  std::shared_ptr<DB_Interface> db;
  std::shared_ptr<StatusBarNCurses> status;
  /** @brief Handle to the date range selector. */
  DateSelectorNcurses date_selector;
  /** @brief Register displaying the weekly totals*/
  WeeklyReportRegister reg;
};

#endif // WEEKLY_REPORT_SCREEN_H
