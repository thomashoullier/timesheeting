#include "column.h"
#include <form.h>
#include <iostream>

Column::Column(std::vector<std::string> fields_str) {
  // Construct the fields vector
  const auto height {1};
  const auto leftcol {1};
  const auto width {26};

  for (std::size_t i = 0 ; i < fields_str.size() ; ++i) {
    fields[i] = new_field(height, width-3, i, leftcol, 0, 0);
    set_field_buffer(fields[i], 0, fields_str.at(i).c_str());
  }
  fields[3] = NULL;
  /* Create the form */
  form = new_form(fields);
  /* Create window */
  int rows, cols;
  scale_form(form, &rows, &cols);
  win = newwin(rows+2, width, 1, 1);
  set_form_win(form, win);
  set_form_sub(form, derwin(win, rows, width-2, 1, 1));
  box(win, 0, 0);
  /* Refresh form */
  post_form(form);
  set_current_field(form, fields[0]);
  wrefresh(win);
}

Column::~Column() {
  unpost_form(form);
  free_form(form);
  free_field(fields[0]);
  free_field(fields[1]);
  free_field(fields[2]);
}

void Column::next_field () {
  form_driver(form, REQ_NEXT_FIELD);
}

void Column::prev_field () {
  form_driver(form, REQ_PREV_FIELD);
}

//void Column::refresh() {}
