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
  // void refresh();

private:
  ColPos pos;       // Column position on the screen: 0, 1 or 2.
  FORM *form;
  //FIELD *fields[4];
  static constexpr std::size_t PAGE_LINES {35}; // Number of lines in page.
  std::vector<FIELD*> fields;
};
