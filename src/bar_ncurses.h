#ifndef BAR_NCURSES_H
#define BAR_NCURSES_H

#include <string>
#include "win_ncurses.h"

/** @brief A bar is a window with a single line, which can print strings. */
class BarNCurses : public WinNCurses {
public:
  void print (const std::string &msg);
};

#endif // BAR_NCURSES_H
