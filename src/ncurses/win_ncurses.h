/** @file
 * @brief WinNCurses definition. */
#ifndef WIN_NCURSES_H
#define WIN_NCURSES_H

#include <ncurses.h>
#include "../logger/logger_file.h"

/** @brief Selector for the position of the window. */
enum class WindowPosition { top, upper, lower, bottom, left, middle };
/** @brief Selector for the format of the window. */
enum class WindowFormat { line, block, column };

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
  /** @brief Handle to the logger. */
  // TODO: Grab the logger from the generic class handing out loggers.
  LoggerFile *logger;
};

#endif // WIN_NCURSES_H
