#include "db_interface.h"

DB_Interface::DB_Interface (std::string db_file) {
  auto rc = sqlite3_open(db_file.c_str(), &db);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Cannot open database file.");
  }
}

DB_Interface::~DB_Interface() { sqlite3_close(db); }

std::vector<Project> DB_Interface::query_projects() {
  Project p1 = {1, "JWST", "X"};
  Project p2 = {23, "Ariane 6", "X"};
  Project p3 = {3, "Tondeuse", "X"};
  Project p4 = {4, "SPICA", "X"};
  Project p5 = {5, "Roman", "X"};
  return {p1, p2, p3, p4, p5};
}

std::vector<Task> DB_Interface::query_tasks(Id project_id) {
  std::string id = std::to_string(project_id);
  Task t1 = {1, "Kick-off meeting " + id};
  Task t2 = {2, "Writing specification"};
  Task t3 = {3, "Progress meeting"};
  return {t1, t2, t3};
}

void DB_Interface::create_projects_table() {
  std::string create_projects_table_st =
    "CREATE TABLE projects ("
    "id INTEGER PRIMARY KEY,"
    "number TEXT NOT NULL,"
    "name TEXT NOT NULL"
    ");";
  auto rc = sqlite3_exec(db, create_projects_table_st.c_str(),
                         NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error creating projects table.");
  }
}

void DB_Interface::add_project(std::string project_name) {
  std::string add_project_st =
    "INSERT INTO projects (number, name)"
    "VALUES ('PRJXXX','" + project_name + "');";
  auto rc = sqlite3_exec(db, add_project_st.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error adding project row.");
  }
}
