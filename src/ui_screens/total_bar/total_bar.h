/** @file
 * @brief TotalBar definition. */
#ifndef TOTAL_BAR_H
#define TOTAL_BAR_H

#include <string>
#include "time_lib/duration.h"
#include "ncurses_lib/bar_ncurses.h"

/** @brief Bar for displaying a duration. */
class TotalBar : public ncurses_lib::BarNCurses {
private:
  /** @brief Internal representation of the current duration. */
  // TODO: duration type. Do we even need to hold it?
  //       Can't we just refresh the window and keep the string
  //       we had printed?
  time_lib::Duration duration;
  /** @brief Display string for the duration. */
  std::string display_string;

public:
  /** @brief Construct the bar and initialize the duration. */
  TotalBar (const time_lib::Duration &_duration);
  /** @brief Update the duration. */
  void update(const time_lib::Duration &_duration);
  /** @brief Refresh the display. */
  void refresh() const;

};

#endif // TOTAL_BAR_H
