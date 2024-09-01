#include "period_selector_ui.h"
#include "period_selector_ncurses.h"

PeriodSelectorUI::PeriodSelectorUI
(std::shared_ptr<StatusBarNCurses> _status)
  : period_selector(DateRange(Date(DatePoint::YearBegin), Date())),
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
    case 'r':
      try {
        rename_item();
        update();
        return '\n'; // Update above too.
      } catch (DateParsingFailure &e) {
        status->print_wait("Failed to parse the date. Do nothing.");
        this->clear();
        this->refresh();
      }
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

DateRange PeriodSelectorUI::get_current_date_range() {
  return period_selector.get_current_date_range();
}

void PeriodSelectorUI::rename_item() {
  auto new_str = status->get_user_string();
  auto field_type = period_selector.get_field_type();
  switch (field_type) {
  case PeriodSelectorNCurses::PeriodField::start: {
    Date new_start_date(new_str);
    period_selector.set_start(new_start_date);
  } break;
  case PeriodSelectorNCurses::PeriodField::stop: {
    Date new_stop_date(new_str);
    period_selector.set_stop(new_stop_date);
  }
    break;
  default:
    throw std::logic_error("Don't know what to do for renaming this unknown "
                           "field type");
  }
}
