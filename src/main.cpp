#include <form.h>
#include <iostream>
#include "column.h"

int main() {
  std::cout << "timesheeting" << std::endl;

  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  Column project_column ({"0", "1", "2", "3", "4"});

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
