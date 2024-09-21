/** @file
 * @brief TotalBar definition. */
#ifndef TOTAL_BAR_H
#define TOTAL_BAR_H

#include <string>
#include "../../data_objects/duration.h"
#include "../../ncurses/bar_ncurses.h"

/** @brief Bar for displaying a duration. */
class TotalBar : public BarNCurses {
private:
  /** @brief Internal representation of the current duration. */
  // TODO: duration type. Do we even need to hold it?
  //       Can't we just refresh the window and keep the string
  //       we had printed?
  Duration duration;
  /** @brief Display string for the duration. */
  std::string display_string;

public:
  /** @brief Construct the bar and initialize the duration. */
  TotalBar (const Duration &_duration);
  /** @brief Update the duration. */
  void update(const Duration &_duration);
  /** @brief Refresh the display. */
  void refresh() const;

};

#endif // TOTAL_BAR_H
