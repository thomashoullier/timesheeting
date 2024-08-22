/** @file
 * @brief Status bar interface implementation using ncurses. */
#ifndef STATUS_BAR_NCURSES_H
#define STATUS_BAR_NCURSES_H

#include "../status_bar_interface.h"
#include "../bar_ncurses.h"

/** @brief Status bar implementation using ncurses. */
class StatusBarNCurses : public StatusBarInterface,
                         public BarNCurses {
public:
  StatusBarNCurses();

  void print (const std::string &msg) override;
  void print_wait(const std::string &msg) override;
  bool query_confirmation (const std::string &msg) override;
};

#endif // STATUS_BAR_NCURSES_H
