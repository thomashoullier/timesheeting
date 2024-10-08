#ifndef PERIOD_SELECTOR_NCURSES_H
#define PERIOD_SELECTOR_NCURSES_H

#include "../../ncurses/menu_ncurses.h"
#include "../../data_objects/date_range.h"


class PeriodSelectorNCurses : public MenuNCurses {
public:
  /** @brief Enum for reporting the field type. */
  enum PeriodField { start = 0, stop = 1 };

  /** @brief Constructor. */
  PeriodSelectorNCurses(const DateRange &_period);
  /** @brief Get the field type for the currently selected field. */
  PeriodField get_field_type();
  /** @brief Set the period start date. */
  void set_start(const Date &new_start_date);
  /** @brief Set the period stop date. */
  void set_stop(const Date &new_stop_date);
  /** @brief Return the current date range. */
  DateRange get_current_date_range() const;

private:
  /** @brief Currently selected period. */
  DateRange period;
  /** @brief Update the display from the held period. */
  void update();
};

#endif // PERIOD_SELECTOR_NCURSES_H
