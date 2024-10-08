/** @file
 * @brief BarNCurses definition. */
#ifndef BAR_NCURSES_H
#define BAR_NCURSES_H

#include <string>
#include "win_ncurses.h"

/** @brief A bar is a ncurses window with a single line,
    which can print strings. */
class BarNCurses : public WinNCurses {
public:
  /** @brief Construct a bar at the given position. */
  BarNCurses(WindowPosition winpos, WindowFormat winformat);
  /** @brief Print a string in the bar. */
  void print(const std::string &msg) const;
  /** @brief Print a string to the right of the bar. */
  void print_right(const std::string &msg) const;
  /** @brief Get a new string from the user */
  std::string get_user_string();

private:
  /** @brief Filter the kind of WindowFormat to only lines. */
  WindowFormat filter_window_format(WindowFormat winformat);
  /** @brief Sanitize the strings inputted by the users. */
  std::string sanitize_input(const std::string &input) const;
};

#endif // BAR_NCURSES_H
