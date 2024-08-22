#ifndef WIN_NCURSES_H
#define WIN_NCURSES_H

#include <ncurses.h>

/** @brief Selector for the position of the window. */
enum class WindowPosition { top, upper, bottom };
/** @brief Selector for the format of the window. */
enum class WindowFormat { line, block };

/** @brief Base class for NCurses elements in a window. */
class WinNCurses {
public:
  WinNCurses(WindowPosition winpos, WindowFormat winformat);
  ~WinNCurses();

  /** @brief Get a character inputted in the current window. */
  char get_input();
  /** @brief Refresh the display. */
  void refresh() const;
  /** @brief Clear the display. */
  void clear() const;

protected:
  WINDOW *win;

private:
  /** @brief Get a new ncurses window. */
  WINDOW* init_window(WindowPosition winpos, WindowFormat winformat);
};

#endif // WIN_NCURSES_H
