/** @file
 * @brief WinNCurses definition. */
#ifndef WIN_NCURSES_H
#define WIN_NCURSES_H

#include <ncurses.h>

namespace ncurses_lib {
  /** @brief Character signalling a screen resize happened. */
  constexpr int screen_resize_event = KEY_RESIZE;

  /** @brief Base class for NCurses elements in a window. */
  class WinNCurses {
  public:
    /** @brief Construct the window with given position and format. */
    explicit WinNCurses();
    /** @brief Destructor. */
    ~WinNCurses();

    /** @brief Get a character inputted by the user in the current window. */
    int get_input();
    /** @brief Refresh the display. */
    void refresh() const;
    /** @brief Clear the display. */
    void clear() const;

  protected:
    /** Ncurses window object. */
    WINDOW *win;

  private:
    /** @brief Get a new ncurses window. */
    WINDOW* init_window();
  };
}
#endif // WIN_NCURSES_H
