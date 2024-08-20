#ifndef STOPWATCH_NCURSES_H
#define STOPWATCH_NCURSES_H

#include "data_objects.h"
#include <ncurses.h>
#include <menu.h>
#include <vector>
#include <string>

/** @brief Ncurses implementation of a stopwatch for generating new entries. */
class StopwatchNcurses {
public:
  StopwatchNcurses (const std::string &str);
  ~StopwatchNcurses();

  /** @brief Refresh the display. */
  void refresh();
  /** @brief Clear the display. */
  void clear();
  /** @brief Select the next item. */
  void select_next_item();
  /** @brief Select the previous item. */
  void select_previous_item();

private:
  static constexpr int WIDTH {80};
  static constexpr int ncols {4};
  std::string tmp_str; // TEMP
  WINDOW *win;
  MENU *menu;
  std::vector<ITEM *> menu_items;
  std::vector<Entry> held_items;

  void init_menu(const std::string &str);
  void init_items(const std::string &str);
  void init_menu_window();
  void destroy_menu();
};

#endif // STOPWATCH_NCURSES_H
