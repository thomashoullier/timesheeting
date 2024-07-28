#ifndef COLUMN_NCURSES_H
#define COLUMN_NCURSES_H

#include "column_interface.h"
#include <curses.h>
#include <menu.h>

template <typename T>
class ColumnNcurses : public ColumnInterface<T> {
public:
  ColumnNcurses (const std::vector<T> &items, ColPos _pos) {
    this->pos = _pos;
    init_form(items);
  }

  ~ColumnNcurses () {
    destroy_form();
  }

  void refresh () override {
    wrefresh(win);
  }

  void set_items(const std::vector<T> &items) override {
    destroy_form();
    init_form(items);
  }

  Id get_current_id() override {
    auto field_index = get_field_index();
    return held_items.at(field_index).id;
  }

  void select_next_item() override { menu_driver(form, REQ_NEXT_ITEM); }

  void select_prev_item() override { menu_driver(form, REQ_PREV_ITEM); }

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
  static constexpr int WIDTH{26};              // Column window width
  static constexpr std::size_t PAGE_LINES{35}; // Number of lines in page.
  WINDOW *win;
  MENU *form;
  std::vector<T> held_items; // A copy of the held items.
  std::vector<ITEM *> fields;

  /** Get the vector index of the currently selected field. */
  int get_field_index() {
    return item_index(current_item(form));
  }

  void init_form_window() {
    win = newwin(PAGE_LINES + 1, WIDTH, 1, 1 + this->pos * WIDTH);
    set_menu_win(form, win);
    set_menu_sub(form, derwin(win, PAGE_LINES, WIDTH - 2, 1, 1));
    set_menu_format(form, PAGE_LINES - 1, 1);
    box(win, 0, 0);
  }

  void destroy_form() {
    unpost_menu(form);
    free_menu(form);
    delwin(win);
    for (auto &f : fields) {
      free_item(f);
    }
    fields.clear();
    held_items.clear();
  }

  void init_fields(const std::vector<T> &field_items) {
    held_items.resize(field_items.size());
    for (std::size_t i = 0; i < field_items.size(); ++i) {
      held_items.at(i) = field_items.at(i); // Copy into internal structure.
      fields.push_back(new_item(held_items.at(i).name.c_str(), NULL));
    }
    fields.push_back(NULL);
    /* Create the form with a pointer into fields. */
    form = new_menu(fields.data());
  }

  void init_form(const std::vector<T> &field_items) {
    init_fields(field_items);
    init_form_window();
    post_menu(form);
    set_current_item(form, fields.at(0));
    refresh();
  }

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
