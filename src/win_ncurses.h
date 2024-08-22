#ifndef WIN_NCURSES_H
#define WIN_NCURSES_H

#include <ncurses.h>

/** @brief Base class for NCurses elements in a window. */
class WinNCurses {
public:
  WinNCurses();
  ~WinNCurses();

  /** @brief Get a character inputted in the current window. */
  char get_input();
  /** @brief Refresh the display. */
  void refresh();
  /** @brief Clear the display. */
  void clear();

protected:
  WINDOW *win;

private:
  /** @brief Get a new ncurses window. */
  WINDOW* init_window();
};

#endif // WIN_NCURSES_H
