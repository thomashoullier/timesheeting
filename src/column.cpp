#include "column.h"

Column::Column(std::vector<std::string> fields_str, ColPos _pos) {
  pos = _pos;
  init_form(fields_str);
}

Column::~Column() { destroy_form(); }

void Column::next_field() { form_driver(form, REQ_NEXT_FIELD); }

void Column::prev_field() { form_driver(form, REQ_PREV_FIELD); }

void Column::recreate_form(std::vector<std::string> fields_str) {
  destroy_form();
  init_form(fields_str);
}

void Column::init_fields(std::vector<std::string> fields_str) {
  const auto height{1};
  const auto leftcol{1};

  for (std::size_t i = 0; i < fields_str.size(); ++i) {
    fields.push_back(new_field(height, WIDTH - 3, i, leftcol, 0, 0));
    set_field_buffer(fields.back(), 0, fields_str.at(i).c_str());
  }
  fields.push_back(NULL);
  /* Create the form with a pointer into fields. */
  form = new_form(fields.data());
}

void Column::init_form_window(ColPos pos) {
  win = newwin(PAGE_LINES + 1, WIDTH, 1, 1 + pos * WIDTH);
  set_form_win(form, win);
  set_form_sub(form, derwin(win, PAGE_LINES, WIDTH - 2, 1, 1));
  box(win, 0, 0);
}

void Column::refresh() {
  post_form(form);
  set_current_field(form, fields.at(0));
  wrefresh(win);
}

void Column::init_form(std::vector<std::string> fields_str) {
  init_fields(fields_str);
  init_form_window(pos);
  refresh();
}

void Column::destroy_form() {
  unpost_form(form);
  free_form(form);
  for (auto f : fields) {
    free_field(f);
  }
  fields.clear();
}
