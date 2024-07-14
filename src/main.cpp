#include <form.h>
#include <iostream>

int main() {
  std::cout << "timesheeting" << std::endl;

  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  /* Creating fields */
  FIELD *proj_field[4];
  auto height {1};
  auto width {23};
  auto leftcol {1};

  proj_field[0] = new_field(height, width, 0, leftcol, 0, 0);
  set_field_buffer(proj_field[0], 0, "Project 0");
  proj_field[1] = new_field(height, width, 1, leftcol, 0, 0);
  set_field_buffer(proj_field[1], 0, "Project 1");
  proj_field[2] = new_field(height, width, 2, leftcol, 0, 0);
  set_field_buffer(proj_field[2], 0, "Project 2");
  proj_field[3] = NULL;

  /* Creating form */
  int rows, cols;
  auto my_form = new_form(proj_field);
  scale_form(my_form, &rows, &cols);

  WINDOW *proj_win;
  proj_win = newwin(rows+2, width+3, 1, 1);
  set_form_win(my_form, proj_win);
  set_form_sub(my_form, derwin(proj_win, rows, width+1, 1, 1));
  box(proj_win, 0, 0);

  post_form(my_form);
  set_current_field(my_form, proj_field[0]);
  wrefresh(proj_win);

  /* Input loop */
  char ch;
  while((ch = wgetch(proj_win)) != 'q') {
    switch(ch) {
    case 'n':
      form_driver(my_form, REQ_NEXT_FIELD);
      break;
    case 'e':
      form_driver(my_form, REQ_PREV_FIELD);
      break;
    case 'k': /* Add a new field. */
      break;
    default:
      break;
    }
  }

  /* Free */
  unpost_form(my_form);
  free_form(my_form);
  free_field(proj_field[0]);
  free_field(proj_field[1]);
  endwin();
}
