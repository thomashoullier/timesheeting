/** @file
 *  @brief PeriodSelectorNCurses definition. */
#ifndef PERIOD_SELECTOR_NCURSES_H
#define PERIOD_SELECTOR_NCURSES_H

#include "ncurses_lib/menu_ncurses.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include <memory>
#include <vector>
// IWYU pragma: no_include <format>

namespace tui {
  /** @brief Menu with two cells for specifying a DateRange using the UI. */
  class PeriodSelectorNCurses final : public ncurses_lib::MenuNCurses {
  public:
    /** @brief Enum for reporting the field type. */
    enum PeriodField { start = 0, stop = 1 };

    /** @brief Constructor. */
    explicit PeriodSelectorNCurses(const time_lib::DateRange &_period);
    /** @brief Get the field type for the currently selected field. */
    PeriodField get_field_type();
    /** @brief Set the period start date. */
    void set_start(const time_lib::Date &new_start_date);
    /** @brief Set the period stop date. */
    void set_stop(const time_lib::Date &new_stop_date);
    /** @brief Return the current date range. */
    time_lib::DateRange get_current_date_range() const;

  private:
    /** @brief Currently selected period. */
    time_lib::DateRange period;
    /** @brief Update the display from the held period. */
    void update();
    /** @brief Period to menu items. */
    std::shared_ptr<std::vector<ncurses_lib::MenuItem>>
    period_to_menu (time_lib::DateRange period);
  };
} // namespace tui

#endif // PERIOD_SELECTOR_NCURSES_H
