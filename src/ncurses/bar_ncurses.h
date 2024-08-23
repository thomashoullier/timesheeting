#ifndef BAR_NCURSES_H
#define BAR_NCURSES_H

#include <string>
#include "win_ncurses.h"

/** @brief A bar is a window with a single line, which can print strings. */
class BarNCurses : public WinNCurses {
public:
  BarNCurses(WindowPosition winpos);
  void print (const std::string &msg) const;
};

#endif // BAR_NCURSES_H
