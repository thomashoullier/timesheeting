#ifndef COLUMN_H
#define COLUMN_H

#include <form.h>
#include <vector>
#include <string>

class Column {
public:
  enum ColPos { left = 0, middle = 1, right = 2 };
  WINDOW *win;

  Column(const std::vector<std::string> &fields_str, ColPos pos);
  ~Column();

  void next_field();
  void prev_field();
  void recreate_form(const std::vector<std::string> &fields_str);
  void refresh();

private:
  ColPos pos; // Position of the column on the screen.
  FORM *form;
  static constexpr int WIDTH {26}; // Colum window width.
  static constexpr std::size_t PAGE_LINES {35}; // Number of lines in page.
  std::vector<FIELD*> fields;

  void init_fields (const std::vector<std::string> &fields_str);
  void init_form_window (ColPos pos);
  void init_form(const std::vector<std::string> &fields_str);
  void destroy_form();
};

#endif // COLUMN_H
