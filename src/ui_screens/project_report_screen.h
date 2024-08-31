#ifndef PROJECT_REPORT_SCREEN_H
#define PROJECT_REPORT_SCREEN_H

#include "period_selector/period_selector_ui.h"
#include "project_totals_register/project_totals_register.h"
#include "status_bar/status_bar_ncurses.h"
#include "ui_component.h"
#include "../db_interface.h"
#include <memory>

template <typename T_DB,
          typename = std::enable_if_t<std::is_base_of_v<DB_Interface, T_DB>>>
class ProjectReportScreen : public UIComponent {
public:
  explicit ProjectReportScreen(std::shared_ptr<T_DB> _db,
                               std::shared_ptr<StatusBarNCurses> _status)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      status(_status),
      period_selector_ui(PeriodSelectorUI(_status)),
      reg(ProjectTotalsRegister
          (db->report_project_totals
           (period_selector_ui.get_current_date_range())))
  {};

  char input_loop() override {
    UIComponent *cur_focus {&period_selector_ui};
    cur_focus = &reg;
    while (true) {
      auto ch = cur_focus->input_loop();
      switch(ch) {
      case '\t':
        if (cur_focus == &period_selector_ui)
          cur_focus = &reg;
        else
          cur_focus = &period_selector_ui;
        break;
      case '\n': // Update request is passed
        this->update();
        break;
      default:
        return ch;
      }
    }
  };

  void refresh() override {
    period_selector_ui.refresh();
    reg.refresh();
  };
  void clear() override {
    period_selector_ui.clear();
    reg.clear();
  };

  void update() override {
    period_selector_ui.update();
    auto cur_range = period_selector_ui.get_current_date_range();
    reg.set_items(db->report_project_totals(cur_range));
    reg.update();
  };

private:
  std::shared_ptr<DB_Interface>  db;
  std::shared_ptr<StatusBarNCurses> status;
  PeriodSelectorUI period_selector_ui;
  ProjectTotalsRegister reg;
};

#endif // PROJECT_REPORT_SCREEN_H
