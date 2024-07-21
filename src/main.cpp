#include "db_interface.h"
#include "project_task_table.h"
#include "logger.h"
#include <form.h>
#include <iostream>
#include <memory>

int main() {
  std::cout << "timesheeting" << std::endl;

  /* Logger */
  Logger log ("timesheeting.log");
  log.log("Hello world.");

  /* SQLite */
  auto db = std::make_shared<DB_Interface>("timesheeting.db");
  db->create_projects_table();
  db->add_project("JWST");

  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  ProjectTaskTable projects_table (db);
  projects_table.input_loop();

  endwin();
}
