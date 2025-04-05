/** @file
 * @brief WinNCurses definition. */
#ifndef WIN_NCURSES_H
#define WIN_NCURSES_H

#include <array>
#include <ncurses.h>
#include <string>

namespace ncurses_lib {
  /** @brief Possible appearances for the display of strings. */
  enum class StringFace {
    Normal,
    Bold,
    Italic
  };

  /** @brief Selector for the position of the window. */
  enum class WindowPosition { top, top_left, top_right,
                              upper, lower, bottom, left, middle };
  /** @brief Selector for the format of the window. */
  enum class WindowFormat { line, half_line, box, block, column };

  /** @brief Character signalling a screen resize happened. */
  constexpr int screen_resize_event = KEY_RESIZE;

  /** @brief Base class for NCurses elements in a window. */
  class WinNCurses {
  public:
    /** @brief Construct the window with given position and format. */
    explicit WinNCurses(WindowPosition _winpos, WindowFormat _winformat);
    /** @brief Destructor. */
    virtual ~WinNCurses();

    /** @brief Get a character inputted by the user in the current window. */
    int get_input();
    /** @brief Refresh the display. */
    virtual void refresh();
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
    /** @brief Window position in the standard screen. */
    WindowPosition winpos;
    /** @brief Window format. */
    WindowFormat winformat;
    /** @brief Border drawing toggle. */
    bool border_on;

    /** @brief Compute the window position and dimensions.

        ny, nx, y, x */
    std::array<int, 4> compute_window_dimensions () const;
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
