#include <form.h>
#include <iostream>
#include "project_task_table.h"

int main() {
  std::cout << "timesheeting" << std::endl;

  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  ProjectTaskTable projects_table;
  projects_table.input_loop();

  /* Input loop
  char ch;
  while((ch = wgetch(project_column.win)) != 'q') {
    switch(ch) {
    case 'n':
      project_column.next_field();
      break;
    case 'e':
      project_column.prev_field();
      break;
    default:
      break;
    }
  }*/

  /* Free */
  endwin();
}
