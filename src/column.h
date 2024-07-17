#include <form.h>
#include <vector>
#include <string>

class Column {
public:
  enum ColPos { left = 0, middle = 1, right = 2 };
  WINDOW *win;

  Column(std::vector<std::string> fields_str, ColPos pos);
  ~Column();

  void next_field();
  void prev_field();
  void refresh();

private:
  FORM *form;
  static constexpr int WIDTH {26}; // Colum window width.
  static constexpr std::size_t PAGE_LINES {35}; // Number of lines in page.
  std::vector<FIELD*> fields;

  void init_fields (std::vector<std::string> fields_str);
  void init_form_window (ColPos pos);
  void destroy_form();
};
