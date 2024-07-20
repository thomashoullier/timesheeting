#include <form.h>
#include <iostream>
#include "project_task_table.h"
#include "sqlite3.h"
#include <stdexcept>
#include <string>

int main() {
  std::cout << "timesheeting" << std::endl;

  /* SQLite */
  // Init the db
  sqlite3 *db;
  auto rc = sqlite3_open("timesheeting.db", &db);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Cannot open database file.");
  }

  // Create the project table.
  std::string create_projects_table = "CREATE TABLE projects ("
                                      "id INTEGER PRIMARY KEY,"
                                      "number TEXT NOT NULL,"
                                      "name TEXT NOT NULL"
                                      ");";
  rc = sqlite3_exec(db, create_projects_table.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    throw std::runtime_error("Error creating table.");
  }
  // Adding a row
  std::string add_project = "INSERT INTO projects (number, name)"
                            "VALUES ('PRJ34', 'Ariane');";
  rc = sqlite3_exec(db, add_project.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    throw std::runtime_error("Error adding rows.");
  }

  // Destroying the DB handle.
  sqlite3_close(db);

  /* ncurses init */
  initscr();
  cbreak();
  noecho();

  ProjectTaskTable projects_table;
  projects_table.input_loop();

  endwin();
}
