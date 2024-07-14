#include <form.h>
#include <vector>
#include <string>

class Column {
public:
  WINDOW *win;

  Column(std::vector<std::string> fields_str);
  ~Column();

  void next_field();
  void prev_field();
  // void refresh();

private:
  //int width; // Total width of the window.
  int pos;       // Column position on the screen: 0, 1 or 2.
  FORM *form;
  FIELD *fields[4];
};
