#include "period_selector_ncurses.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include <memory>
// IWYU pragma: no_include <string>

namespace tui {
  PeriodSelectorNCurses::PeriodSelectorNCurses
  (const time_lib::DateRange &_period)
    : MenuNCurses(period_to_menu(_period),
                  ncurses_lib::WindowPosition::lower,
                  ncurses_lib::WindowFormat::box, {0, 0}),
      period(_period) {}

  PeriodSelectorNCurses::PeriodField PeriodSelectorNCurses::get_field_type() {
    return PeriodField(get_item_index());
  }

  void PeriodSelectorNCurses::set_start(const time_lib::Date &new_start_date) {
    period.start = new_start_date;
    this->update();
  }

  void PeriodSelectorNCurses::set_stop(const time_lib::Date &new_stop_date) {
    period.stop = new_stop_date;
    this->update();
  }

  void PeriodSelectorNCurses::update() {
    MenuNCurses::set_items(period_to_menu(period));
  }

  time_lib::DateRange PeriodSelectorNCurses::get_current_date_range() const {
    return period;
  }

  std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
  PeriodSelectorNCurses::period_to_menu(time_lib::DateRange period) {
    auto menu_items = std::make_shared<std::vector<ncurses_lib::MenuItem>>();
    auto display_strings = period.to_string();
    auto cell_strings = period.to_day_strings();
    menu_items->push_back(
        ncurses_lib::MenuItem(cell_strings.at(0), display_strings.at(0),
                              ncurses_lib::StringFace::Normal));
    menu_items->push_back(
        ncurses_lib::MenuItem(cell_strings.at(1), display_strings.at(1),
                              ncurses_lib::StringFace::Normal));
    return menu_items;
  }
} // namespace tui
