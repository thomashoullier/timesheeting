/** @file
 * @brief WinNCurses definition. */
#ifndef WIN_NCURSES_H
#define WIN_NCURSES_H

#include <ncurses.h>
#include <string>

namespace ncurses_lib {
  /** @brief Possible appearances for the display of strings. */
  enum class StringFace {
    Normal,
    Bold
  };

  /** @brief Horizontal layout of the window. Left side, right sight or full. */
  enum class WindowHorizontal {left, right, full};

  /** @brief Character signalling a screen resize happened. */
  constexpr int screen_resize_event = KEY_RESIZE;

  /** @brief Base class for NCurses elements in a window. */
  class WinNCurses {
  public:
    /** @brief Construct the window with given position and format. */
    explicit WinNCurses(int _top_pos, int _bottom_pos,
                        WindowHorizontal _horizontal_layout);
    /** @brief Destructor. */
    ~WinNCurses();

    /** @brief Get a character inputted by the user in the current window. */
    int get_input();
    /** @brief Refresh the display. */
    void refresh() const;
    /** @brief Clear the display. */
    void clear() const;

    /** @brief Get the current number of lines in the window (size). */
    int n_lines() const;
    /** @brief Get the current number of columns in the window (size). */
    int n_cols() const;
    /** @brief Print a string at the given position. */
    void print_at(const std::string &str,
                  int line, int col_offset, int col_width,
                  StringFace face = StringFace::Normal) const;
    /** @brief Print a string with standout appearance. */
    void print_standout_at(const std::string &str,
                           int line, int col_offset, int col_width,
                           StringFace face = StringFace::Normal) const;
    /** @brief Resize the window according to current terminal size. */
    void resize();
    /** @brief Activate the border. */
    void set_border();
    /** @biref Deactivate the border. */
    void unset_border();

  private:
    /** @brief Position of the top of the window from the top of the screen. */
    int top_position;
    /** @brief Position of the bottom of the window from the bottom
               of the screen. */
    int bottom_position;
    /** @brief Horizontal layout */
    WindowHorizontal horizontal_layout;
    /** @brief Border drawing toggle. */
    bool border_on;

    /** @brief Get a new ncurses window. */
    WINDOW *init_window();
    /** @brief Draw a border at the top of the window. */
    void draw_border() const;

  protected:
    /** Ncurses window object. */
    WINDOW *win;
  };
}
#endif // WIN_NCURSES_H
