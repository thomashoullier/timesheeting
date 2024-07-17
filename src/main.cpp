#include <form.h>
#include <iostream>
#include "column.h"

int main() {
  std::cout << "timesheeting" << std::endl;

  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  // TODO: create a master/slave two-column table for browsing
  //       using the master column which loads a slave column on the right.
  Column project_column({"p0", "p1", "p2", "p3", "p4"}, Column::left);
  Column task_column({"t0", "t1", "t2", "t3"}, Column::middle);

  task_column.recreate_form({"z0", "z1", "z2"});

  /* Input loop */
  char ch;
  while((ch = wgetch(project_column.win)) != 'q') {
    switch(ch) {
    case 'n':
      project_column.next_field();
      break;
    case 'e':
      project_column.prev_field();
      break;
    case 'k': /* Add a new field. */
      break;
    default:
      break;
    }
  }

  /* Free */
  endwin();
}
