#include "db/db_sqlite.h"
#include "logger.h"
#include "project_task_table.h"
#include "column_ncurses.h"
#include "status_bar_ncurses.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
  std::cout << "timesheeting" << std::endl;

  /* Logger */
  Logger log("timesheeting.log");
  log.log("Hello world.");

  /* SQLite */
  auto db = std::make_shared<DB_SQLite>("timesheeting.db");
  db->add_project("JWST");
  auto project_id = db->query_projects().back().id;
  db->add_task(project_id, "Kick-off");
  db->add_task(project_id, "Validation campaign");
  db->add_task(project_id, "Closure meeting");
  /* ncurses init */
  initscr();
  cbreak();
  noecho();
  /* Status bar */
  auto status_bar = std::make_shared<StatusBarNCurses>();

  ProjectTaskTable<DB_SQLite, StatusBarNCurses,
                   ColumnNcurses<Project>, ColumnNcurses<Task>>
    projects_table(db, status_bar);
  projects_table.input_loop();

  endwin();
}
