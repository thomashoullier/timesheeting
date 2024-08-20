#ifndef DATE_SELECTOR_INTERFACE_H
#define DATE_SELECTOR_INTERFACE_H

#include "date_range.h"

/** @brief Virtual interface class for a date range selector UI. */
class DateSelectorInterface {
public:
  virtual ~DateSelectorInterface() = default;
  /** @brief Print the currently selected date range. */
  virtual void print () const = 0;
  /** @brief Refresh the display. */
  virtual void refresh() const = 0;
  /** @brief Clear the display. */
  virtual void clear() const = 0;
  /** @brief Return the currently selected date range. */
  virtual DateRange current_range () const = 0;
  /** @brief Select the next day. */
  virtual void select_next_day () = 0;
  /** @brief Select the previous day. */
  virtual void select_previous_day () = 0;
};

#endif // DATE_SELECTOR_INTERFACE_H
