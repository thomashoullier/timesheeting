/** @file
 * @brief Column interface implementation using ncurses. */
#ifndef COLUMN_NCURSES_H
#define COLUMN_NCURSES_H

#include "../column_interface.h"
#include <menu.h>
#include <ncurses.h>

/** @brief Column implementation in ncurses. */
template <typename T> class ColumnNcurses : public ColumnInterface<T> {
public:
  /** @brief Column constructor. */
  ColumnNcurses(const std::vector<T> &items, ColPos _pos) {
    this->pos = _pos;
    init_menu(items);
  }

  /** @brief Column destructor. */
  ~ColumnNcurses() { destroy_menu(); }

  void refresh() override {
    post_menu(menu);
    wrefresh(win);
  }

  void clear () override {
    wclear(win);
    unpost_menu(menu);
  }

  void set_items(const std::vector<T> &items) override {
    destroy_menu();
    init_menu(items);
  }

  Id get_current_id() override {
    if (held_items.empty()) {
      throw ColumnEmpty("The column is empty!");
    }
    auto menu_index = get_menu_index();
    return held_items.at(menu_index).id;
  }

  std::string get_current_name() override {
    auto menu_index = get_menu_index();
    if (menu_index >= 0)
      return held_items.at(menu_index).name;
    else
      return "";
  }

  void select_next_item() override { menu_driver(menu, REQ_NEXT_ITEM); }

  void select_prev_item() override { menu_driver(menu, REQ_PREV_ITEM); }

  char query_input() override { return wgetch(win); }

  std::string query_new_item_name() override {
    constexpr int INPUT_ROW = PAGE_LINES - 2;
    return query_row_input(INPUT_ROW);
  }

  std::string query_current_item_rename() override {
    auto y = getcury(win);
    return query_row_input(y);
  }

private:
  /** @brief Column window width on the screen. */
  static constexpr int WIDTH{26};
  /** @brief Number of lines on a column page. */
  static constexpr std::size_t PAGE_LINES{35};
  /** @brief Column's ncurses window. */
  WINDOW *win;
  /** @brief Column's ncurses menu. */
  MENU *menu;
  /** @brief List of items held by the column. */
  std::vector<T> held_items;
  /** @brief List of ncurses items in the menu. */
  std::vector<ITEM *> menu_items;

  /** @brief Get the vector index of the currently selected menu item. */
  int get_menu_index() {
    return item_index(current_item(menu));
  }

  /** @brief Setup the ncurses menu window. */
  void init_menu_window() {
    win = newwin(PAGE_LINES + 1, WIDTH, 1, 1 + this->pos * WIDTH);
    set_menu_win(menu, win);
    set_menu_sub(menu, derwin(win, PAGE_LINES, WIDTH - 2, 1, 1));
    set_menu_format(menu, PAGE_LINES - 1, 1);
    box(win, 0, 0);
  }

  /** @brief Destructor for the ncurses objects and reset of all members. */
  void destroy_menu() {
    unpost_menu(menu);
    free_menu(menu);
    delwin(win);
    for (auto &it : menu_items) {
      free_item(it);
    }
    menu_items.clear();
    held_items.clear();
  }

  /** @brief Copy set of items into the internal items vector,
   * and put the items in the ncurses menu. */
  void init_items(const std::vector<T> &items) {
    held_items.resize(items.size());
    for (std::size_t i = 0; i < items.size(); ++i) {
      held_items.at(i) = items.at(i); // Copy into internal structure.
      menu_items.push_back(new_item(held_items.at(i).name.c_str(), NULL));
    }
    menu_items.push_back(NULL);
    menu = new_menu(menu_items.data());
  }

  /** @brief Initialize the ncurses menu. */
  void init_menu(const std::vector<T> &items) {
    init_items(items);
    init_menu_window();
    set_current_item(menu, menu_items.at(0));
  }

  /** @brief Input loop for getting a string from the user. */
  std::string query_row_input (int input_row) {
    std::string input_buffer {};
    // Move cursor to beginning of line.
    wmove(win, input_row, 0);
    // Clear the display line.
    wclrtoeol(win);
    bool user_wants_to_input = true;
    while (user_wants_to_input) { // Item input loop.
      auto ch = wgetch(win);
      switch (ch) {
      case '\n': // User validates the input.
        user_wants_to_input = false;
        break;
      case 27: // User wants to cancel.
        input_buffer.clear();
        user_wants_to_input = false;
        break;
      case 127: // Erase last character
        if (!input_buffer.empty()) {
          input_buffer.pop_back();
          // Remove character from screen
          int y, x;
          getyx(win, y, x);
          auto xmax = getmaxx(win);
          if (x == 0) { // jump to the end of the previous line.
            wmove(win, y-1, xmax-1);
          } else {
            wmove(win, y, x - 1);
          }
          wdelch(win);
        }
        break;
      default: // Gets added to item.
        input_buffer.push_back(ch);
        waddch(win, ch);
        break;
      }
    }
    // Cleanup the input row.
    wmove(win, input_row, 0);
    wclrtoeol(win);
    return input_buffer;
  }
};

#endif // COLUMN_NCURSES_H
