/** @file
 * @brief Status bar interface implementation using ncurses. */
#ifndef STATUS_BAR_NCURSES_H
#define STATUS_BAR_NCURSES_H

#include "../../ncurses/bar_ncurses.h"

/** @brief Status bar implementation using ncurses. */
class StatusBarNCurses : public BarNCurses {
public:
  StatusBarNCurses();

  /** @brief Print the message in the status bar. */
  void print(const std::string &msg);
  /** @brief Print the message in the status bar, and then pause until user
   * input. */
  void print_wait(const std::string &msg);
  /** @brief Ask for confirmation. */
  bool query_confirmation (const std::string &msg);
};

#endif // STATUS_BAR_NCURSES_H
