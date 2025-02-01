#ifndef MENU_NCURSES_H
#define MENU_NCURSES_H

#include "win_ncurses.h"
#include <string>
#include <vector>
#include <memory>

namespace ncurses_lib {
  class MenuNCurses: public WinNCurses {
  public:
    explicit MenuNCurses();

    /**@brief Print all items to screen. */
    void print_items ();

  };
}


#endif // MENU_NCURSES_H
