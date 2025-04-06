#include "period_selector_ui.h"
#include "config/key.h"
#include "ncurses_lib/menu_ncurses.h"
#include "period_selector_ncurses.h"
#include "ui/keys/bound_keys.h"
#include "../status_bar/status_bar.h"
#include <stdexcept>
#include <string>
#include "config/binding_maps.h"
#include "time_lib/date.h"

namespace tui {
  PeriodSelectorUI::PeriodSelectorUI ()
    : period_selector(time_lib::DateRange
                      (time_lib::Date(time_lib::DatePoint::YearBegin),
                       time_lib::Date())) {}

  config::NormalActions PeriodSelectorUI::input_loop() {
    period_selector.set_border();
    update_status();
    while (true) {
      auto ch = period_selector.get_input();
      auto action = keys::BoundKeys::get().kb.normal_mode.action_requested(ch);
      switch(action) {
      case config::NormalActions::left:
        if(period_selector.select_left_item() ==
           ncurses_lib::MenuNCurses::ItemSelectionStatus::changed)
          update_status();
        break;
      case config::NormalActions::right:
        if (period_selector.select_right_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed)
          update_status();
        break;
      case config::NormalActions::rename:
        try {
          rename_item();
          update();
          update_status();
          return action;
        } catch (time_lib::DateParsingFailure &e) {
          status().print_wait("Failed to parse the date. Do nothing.");
          update_status();
        }
        break;
      case config::NormalActions::subtabs:
      case config::NormalActions::entries_screen:
      case config::NormalActions::projects_screen:
      case config::NormalActions::locations_screen:
      case config::NormalActions::weekly_report_screen:
      case config::NormalActions::duration_display:
      case config::NormalActions::quit:
        period_selector.unset_border();
        return action;
        break;
      default: // Do nothing
        break;
      }
    }
  }

  void PeriodSelectorUI::refresh() { period_selector.refresh(); }
  void PeriodSelectorUI::clear() { period_selector.clear(); }
  void PeriodSelectorUI::resize() { period_selector.resize(); }
  void PeriodSelectorUI::update() { this->refresh(); }

  void PeriodSelectorUI::update_status() {
    status().print(period_selector.get_current_item_string());
  }

  time_lib::DateRange PeriodSelectorUI::get_current_date_range() {
    return period_selector.get_current_date_range();
  }

  void PeriodSelectorUI::rename_item() {
    auto new_str = status().get_user_string();
    auto field_type = period_selector.get_field_type();
    switch (field_type) {
    case PeriodSelectorNCurses::PeriodField::start: {
      time_lib::Date new_start_date(new_str);
      period_selector.set_start(new_start_date);
    } break;
    case PeriodSelectorNCurses::PeriodField::stop: {
      time_lib::Date new_stop_date(new_str);
      period_selector.set_stop(new_stop_date);
    }
      break;
    default:
      throw std::logic_error("Don't know what to do for renaming this unknown "
                             "field type");
    }
  }
} // namespace tui
