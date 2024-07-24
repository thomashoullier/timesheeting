#include "db_sqlite.h"
#include "logger.h"
#include "project_task_table.h"
#include <form.h>
#include <iostream>
#include <memory>

int main() {
  std::cout << "timesheeting" << std::endl;

  /* Logger */
  Logger log("timesheeting.log");
  log.log("Hello world.");

  /* SQLite */
  auto db = std::make_shared<DB_SQLite>("timesheeting.db");
  // TODO: not part of the DB interface.
  db->create_projects_table();
  db->create_tasks_table();
  db->add_project("JWST");
  auto project_id = db->query_projects().at(0).id;
  db->add_task(project_id, "Kick-off");
  db->add_task(project_id, "Validation campaign");
  db->add_task(project_id, "Closure meeting");
  auto tasks = db->query_tasks(project_id);
  log.log(tasks.at(1).name);
  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  ProjectTaskTable projects_table (db);
  projects_table.input_loop();

  endwin();
}
