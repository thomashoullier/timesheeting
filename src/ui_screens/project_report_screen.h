#ifndef PROJECT_REPORT_SCREEN_H
#define PROJECT_REPORT_SCREEN_H

#include "period_selector/period_selector_ncurses.h"
#include "period_selector/period_selector_ui.h"
#include "status_bar/status_bar_ncurses.h"
#include "ui_component.h"
#include <memory>

class ProjectReportScreen : public UIComponent {
public:
  explicit ProjectReportScreen(std::shared_ptr<StatusBarNCurses> _status)
    : period_selector_ui(std::make_unique<PeriodSelectorUI>(_status)) {};

  char input_loop() override {
    UIComponent *cur_focus {period_selector_ui.get()};
    while (true) {
      auto ch = cur_focus->input_loop();
      switch(ch) {
      default:
        return ch;
      }
    }
  };

  void refresh() override {
    period_selector_ui->refresh();
  };
  void clear() override {
    period_selector_ui->clear();
  };
  void update() override {
    period_selector_ui->update();
  };

private:
  std::unique_ptr<UIComponent> period_selector_ui;
};

#endif // PROJECT_REPORT_SCREEN_H
