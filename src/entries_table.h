#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include <cstddef>
#include <curses.h>

// TODO: make a generic interface to a register-like thing and a
//       ncurses implementation of it. And make this class into
//       a template.
/** @brief Class for holding the table of entries for a given day. */
class EntriesTable {
public:
  EntriesTable () {
    win = newwin(PAGE_LINES, WIDTH, 1, 1);
  };

  ~EntriesTable () {
    delwin(win);
  };

  char input_loop() {
    return wgetch(win);
  };

  void refresh () {
    wmove(win, 0, 0);
    wprintw(win, "Entries screen.");
    wrefresh(win);
  };

  void clear () {
    werase(win);
  };

private:
  static constexpr std::size_t PAGE_LINES{35};
  static constexpr int WIDTH{80};
  WINDOW *win;
};

#endif // ENTRIES_TABLE_H
