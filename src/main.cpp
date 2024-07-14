#include <form.h>
#include <iostream>

int main() {
  std::cout << "timesheeting" << std::endl;

  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  /* Creating fields */
  FIELD *field[4];
  auto height {1};
  auto width {10};
  auto leftcol {1};

  field[0] = new_field(height, width, 4, leftcol, 0, 0);
  set_field_buffer(field[0], 0, "Project 0");
  field[1] = new_field(height, width, 5, leftcol, 0, 0);
  set_field_buffer(field[1], 0, "Project 1");
  field[2] = new_field(height, width, 6, leftcol, 0, 0);
  set_field_buffer(field[2], 0, "Project 2");
  field[3] = NULL;

  /* Creating form */
  auto my_form = new_form(field);
  post_form(my_form);
  set_current_field(my_form, field[0]);

  refresh();

  /* Input loop */
  char ch;
  while((ch = getch()) != 'q') {
    switch(ch) {
    case 'n':
      form_driver(my_form, REQ_NEXT_FIELD);
      break;
    case 'e':
      form_driver(my_form, REQ_PREV_FIELD);
      break;
    default:
      break;
    }
  }

  /* Free */
  unpost_form(my_form);
  free_form(my_form);
  free_field(field[0]);
  free_field(field[1]);
  endwin();
}
