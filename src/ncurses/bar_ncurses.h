/** @file
 * @brief BarNCurses definition. */
#ifndef BAR_NCURSES_H
#define BAR_NCURSES_H

#include <string>
#include "win_ncurses.h"

/** @brief A bar is a ncurses window with a single line,
    which can print strings. */
class BarNCurses : public WinNCurses {
public:
  /** @brief Construct a bar at the given position. */
  BarNCurses(WindowPosition winpos, WindowFormat winformat);
  /** @brief Print a string in the bar. */
  void print (const std::string &msg) const;
private:
  /** @brief Filter the kind of WindowFormat to only lines. */
  WindowFormat filter_window_format(WindowFormat winformat);
};

#endif // BAR_NCURSES_H
