#include "period_selector_ui.h"

PeriodSelectorUI::PeriodSelectorUI
(std::shared_ptr<StatusBarNCurses> _status)
  : period_selector(DateRange(Date(), Date())),
    status(_status) {}

char PeriodSelectorUI::input_loop() {
  period_selector.set_border();
  while (true) {
    status->print(period_selector.get_current_item_string());
    auto ch = period_selector.get_input();
    switch(ch) {
    case 'h':
      period_selector.select_left_item();
      break;
    case 'i':
      period_selector.select_right_item();
      break;
    default:
      period_selector.unset_border();
      return ch;
    }
  }
}

void PeriodSelectorUI::refresh() { period_selector.refresh(); }
void PeriodSelectorUI::clear() { period_selector.clear(); }
void PeriodSelectorUI::update() { this->refresh(); }
