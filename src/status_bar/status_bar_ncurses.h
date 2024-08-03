/** @file
 * @brief Status bar interface implementation using ncurses. */
#ifndef STATUS_BAR_NCURSES_H
#define STATUS_BAR_NCURSES_H

#include "../status_bar_interface.h"
#include <ncurses.h>

/** @brief Status bar implementation using ncurses. */
class StatusBarNCurses : public StatusBarInterface {
public:
  StatusBarNCurses ();
  ~StatusBarNCurses ();
  void print (const std::string &msg) override;
  void print_wait(const std::string &msg) override;
  bool query_confirmation (const std::string &msg) override;
private:
  /** @brief Handle to the status bar's ncurses window. */
  WINDOW *win;
  /** @brief Setup the ncurses window for the status bar. */
  WINDOW* init_window ();
};

#endif // STATUS_BAR_NCURSES_H
