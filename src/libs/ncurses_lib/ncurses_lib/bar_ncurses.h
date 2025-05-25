/** @file
 * @brief BarNCurses definition. */
#ifndef BAR_NCURSES_H
#define BAR_NCURSES_H

#include <string>
#include <cstddef>
#include "win_ncurses.h"

namespace ncurses_lib {
  /** @brief A bar is a ncurses window with a single line,
      which can print strings. */
  class BarNCurses : public WinNCurses {
  public:
    /** @brief Construct a bar at the given position. */
    explicit BarNCurses(WindowPosition winpos, WindowFormat winformat);
    /** @brief Print a string in the bar. */
    virtual void print(const std::string &msg);
    /** @brief Print a string to the right of the bar. */
    void print_right(const std::string &msg);
    /** @brief Print after the cursor in a different style. */
    void print_after_cursor(const std::string &msg);

    /** @brief Make cursor visible/invisible. */
    void set_cursor_visibility(bool visible);
    /** @brief Prepare the bar for cursor-based input. */
    void prepare_input();
    /** @brief Add character to the bar. */
    void add_char(int ch);
    /** @brief Remove last character from bar. */
    void remove_char();
    /** @brief Get the max size of the bar. */
    std::size_t max_size() const;

  private:
    /** @brief Filter the kind of WindowFormat to only lines. */
    WindowFormat filter_window_format(WindowFormat winformat);
  };
}
#endif // BAR_NCURSES_H
