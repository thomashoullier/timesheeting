#ifndef COLUMN_H
#define COLUMN_H

#include "data_objects.h"
#include <form.h>
#include <type_traits>
#include <vector>

enum ColPos { left = 0, middle = 1, right = 2 };

class ColumnBase {
protected:
  WINDOW *win;
  FORM *form;
  // field ids in 1-1 correspondance with curses fields indexing
  std::vector<Id> field_ids;

public:
  void next_field() { form_driver(form, REQ_NEXT_FIELD); }

  void prev_field() { form_driver(form, REQ_PREV_FIELD); }

  int get_field_index () { return field_index(current_field(form)); }

  // Get the Id for the currently selected field.
  Id get_field_id () {
    auto field_index = get_field_index();
    return field_ids.at(field_index);
  }

  char get_input () { return wgetch(win); }

  void refresh() { wrefresh(win); }
};

template <typename T>
class Column : public ColumnBase {
  static_assert(std::is_base_of<GenericItem, T>::value,
                "Not derived from GenericItem.");

public:
  Column(const std::vector<T> &field_items, ColPos _pos) {
    pos = _pos;
    init_form(field_items);
  }

  ~Column() { destroy_form(); }

  void recreate_form(const std::vector<T> &field_items) {
    destroy_form();
    init_form(field_items);
  }

private:
  ColPos pos; // Position of the column on the screen.
  static constexpr int WIDTH{26};              // Colum window width.
  static constexpr std::size_t PAGE_LINES{35}; // Number of lines in page.
  std::vector<FIELD *> fields;

  void init_form_window(ColPos pos) {
    win = newwin(PAGE_LINES + 1, WIDTH, 1, 1 + pos * WIDTH);
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
    init_form_window(pos);
    post_form(form);
    set_current_field(form, fields.at(0));
    refresh();
  }
};

#endif // COLUMN_H
