/** @file
 * @brief NcursesHandle definition. */
#ifndef NCURSES_HANDLE_H
#define NCURSES_HANDLE_H

#include <ncurses.h>

/** @brief Class for managing the top-level ncurses init and teardown. */
class NcursesHandle {
public:
  /** @brief Constructor. */
  NcursesHandle ();
  /** @brief Destructor. */
  ~NcursesHandle ();
};

#endif // NCURSES_HANDLE_H
