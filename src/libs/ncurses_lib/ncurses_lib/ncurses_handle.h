/** @file
 * @brief NcursesHandle definition. */
#ifndef NCURSES_HANDLE_H
#define NCURSES_HANDLE_H

namespace ncurses_lib {
  /** @brief Class for managing the top-level ncurses init and teardown. */
  class NcursesHandle {
  public:
    /** @brief Constructor. */
    explicit NcursesHandle ();
    /** @brief Destructor. */
    ~NcursesHandle ();
  };
}
#endif // NCURSES_HANDLE_H
