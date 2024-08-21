#ifndef STOPWATCH_NCURSES_H
#define STOPWATCH_NCURSES_H

#include "data_objects.h"
#include <array>
#include <ncurses.h>
#include <menu.h>
#include <string>

/** @brief Ncurses implementation of a stopwatch for generating new entries. */
class StopwatchNcurses {
public:
  StopwatchNcurses (const EntryStaging &_entry_staging);
  ~StopwatchNcurses();

  /** @brief Refresh the display. */
  void refresh();
  /** @brief Clear the display. */
  void clear();
  /** @brief Select the next item. */
  void select_next_item();
  /** @brief Select the previous item. */
  void select_previous_item();
  /** @brief Get an input. */
  char query_input();

private:
  static constexpr int WIDTH {80};
  static constexpr int ncols {4};
  WINDOW *win;
  MENU *menu;
  std::array<ITEM *, ncols + 1> menu_items;
  std::array<std::string, ncols> menu_string;
  EntryStaging entry_staging;

  void init_menu();
  void init_items();
  void init_menu_window();
  void destroy_menu();
};

#endif // STOPWATCH_NCURSES_H
