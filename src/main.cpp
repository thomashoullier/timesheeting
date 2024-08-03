#include "db/db_sqlite.h"
#include "logger/logger_file.h"
#include "logger_interface.h"
#include "project_task_table.h"
#include "column/column_ncurses.h"
#include "status_bar/status_bar_ncurses.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
  std::cout << "timesheeting" << std::endl;

  /* Logger */
  LoggerInterface* logger = &LoggerFile::get();
  logger->log("timesheeting startup.");

  /* SQLite */
  auto db = std::make_shared<DB_SQLite>("timesheeting.db");
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
