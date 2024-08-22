#ifndef MENU_NCURSES_H
#define MENU_NCURSES_H

#include "win_ncurses.h"
#include <menu.h>
#include <vector>
#include <string>

/** @brief A ncurses window holding a generic menu. */
class MenuNCurses : public WinNCurses {
public:
  MenuNCurses(const std::vector<std::string> &items,
              WindowPosition winpos, WindowFormat winformat,
              int _ncols);
  ~MenuNCurses();

  /** @brief Query a new string for the currently selected item. */
  std::string query_current_item_rename();
  /** @brief Select the item down of the current one. */
  void select_down_item();
  /** @brief Select the item up of the current one. */
  void select_up_item();
  /** @brief Select the item right of the current one. */
  void select_right_item();
  /** @brief Select the item left of the current one. */
  void select_left_item();
  /** @brief Refresh the display. */
  void refresh();
  /** @brief Clear the display. */
  void clear();
  /** @brief Replace the currently held items. */
  void set_items(const std::vector<std::string> &items);

protected:
  /** @brief Get the current row index. */
  int get_row_index() const;
  /** @brief Get the current column index. */
  int get_col_index() const;

private:
  /** @brief Number of columms in the menu. */
  int ncols;
  /** @brief ncurses menu object. */
  MENU *menu;
  /** @brief ncurses menu items. */
  std::vector<ITEM *> menu_items;
  /** @brief Persistent storage for the strings being displayed. */
  std::vector<std::string> display_strings;

  /** @brief Constructor helper: initialize the menu. */
  void init_menu(const std::vector<std::string> &items);
  /** @brief Initialize the menu items. */
  void init_items(const std::vector<std::string> &items);
  /** @brief Initialize the menu UI. */
  void init_menu_window();
  /** @brief Destructor helper. Also called on update. */
  void destroy_menu ();
};

#endif // MENU_NCURSES_H
