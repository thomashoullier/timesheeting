/** @file
 * @brief WinNCurses definition. */
#ifndef WIN_NCURSES_H
#define WIN_NCURSES_H

#include <ncurses.h>

namespace ncurses_lib {
  /** @brief Selector for the position of the window. */
  enum class WindowPosition { top, top_left, top_right,
                              upper, lower, bottom, left, middle };
  /** @brief Selector for the format of the window. */
  enum class WindowFormat { line, half_line, box, block, column };

  /** @brief Base class for NCurses elements in a window. */
  class WinNCurses {
  public:
    /** @brief Construct the window with given position and format. */
    WinNCurses(WindowPosition winpos, WindowFormat winformat);
    ~WinNCurses();

    /** @brief Get a character inputted by the user in the current window. */
    char get_input();
    /** @brief Refresh the display. */
    void refresh() const;
    /** @brief Clear the display. */
    void clear() const;

  protected:
    /** Ncurses window object. */
    WINDOW *win;

    /** @brief Destroy the ncurses window. */
    void destroy_window();

  private:
    /** @brief Get a new ncurses window. */
    WINDOW* init_window(WindowPosition winpos, WindowFormat winformat);
  };
}
#endif // WIN_NCURSES_H
