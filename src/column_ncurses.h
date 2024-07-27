#ifndef COLUMN_NCURSES_H
#define COLUMN_NCURSES_H

#include "column_interface.h"
#include <curses.h>
#include <form.h>

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
    return field_ids.at(field_index);
  }

  void select_next_item() override { form_driver(form, REQ_NEXT_FIELD); }

  void select_prev_item() override { form_driver(form, REQ_PREV_FIELD); }

  char query_input() override { return wgetch(win); }

  std::string query_new_item_name() override {
    constexpr int INPUT_ROW = PAGE_LINES - 2;
    return query_row_input(INPUT_ROW);
  }

  std::string query_current_item_rename() override {
    auto y = get_current_row();
    return query_row_input(y);
  }

private:
  static constexpr int WIDTH{26};              // Column window width
  static constexpr std::size_t PAGE_LINES{35}; // Number of lines in page.
  WINDOW *win;
  FORM *form;
  // field ids in 1-1 correspondance with curses fields indexing
  std::vector<Id> field_ids;
  std::vector<FIELD *> fields;

  /** Get the vector index of the currently selected field. */
  int get_field_index() {
    return field_index(current_field(form));
  }

  void init_form_window() {
    win = newwin(PAGE_LINES + 1, WIDTH, 1, 1 + this->pos * WIDTH);
    set_form_win(form, win);
    set_form_sub(form, derwin(win, PAGE_LINES, WIDTH - 2, 1, 1));
    box(win, 0, 0);
  }

  void destroy_form() {
    unpost_form(form);
    free_form(form);
    for (auto f : fields) {
      free_field(f);
    }
    fields.clear();
    field_ids.clear();
  }

  void init_fields(const std::vector<T> &field_items) {
    const auto height{1};
    const auto leftcol{1};

    for (std::size_t i = 0; i < field_items.size(); ++i) {
      fields.push_back(new_field(height, WIDTH - 3, i, leftcol, 0, 0));
      // Name
      set_field_buffer(fields.back(), 0, field_items.at(i).name.c_str());
      // Id
      field_ids.push_back(field_items.at(i).id);
    }
    fields.push_back(NULL);
    /* Create the form with a pointer into fields. */
    form = new_form(fields.data());
  }

  void init_form(const std::vector<T> &field_items) {
    init_fields(field_items);
    init_form_window();
    post_form(form);
    set_current_field(form, fields.at(0));
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
        // TODO: Bug when erasing on a second line.
        if (!input_buffer.empty()) {
          input_buffer.pop_back();
          // Remove character from screen
          int y, x;
          getyx(win, y, x);
          wmove(win, y, x - 1);
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

  /** Get the current row position in the window. */
  int get_current_row () {
    int y;
    [[maybe_unused]] int x;
    getyx(win, y, x);
    return y;
  }

};

#endif // COLUMN_NCURSES_H
