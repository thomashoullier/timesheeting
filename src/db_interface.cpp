#include "db_interface.h"
#include <iostream> // TODO: temp

DB_Interface::DB_Interface (std::string db_file) {
  auto rc = sqlite3_open(db_file.c_str(), &db);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Cannot open database file.");
  }
}

DB_Interface::~DB_Interface() { sqlite3_close(db); }

std::vector<Project> DB_Interface::query_projects() {
  std::string select_projects = "SELECT id, name FROM projects;";
  sqlite3_stmt *stmt;
  auto rc = sqlite3_prepare_v2(db, select_projects.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Could not prepare SQL statement.");
  }
  // Get and parse each of the results in the rows.
  std::vector<Project> projects {};
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Project proj {};
    proj.id = sqlite3_column_int64(stmt, 0);
    auto name = sqlite3_column_text(stmt, 1);
    proj.name = reinterpret_cast<const char*>(name);
    projects.push_back(proj);
  }
  sqlite3_finalize(stmt);
  return projects;
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
    "name TEXT NOT NULL"
    ");";
  auto rc = sqlite3_exec(db, create_projects_table_st.c_str(),
                         NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error creating projects table.");
  }
}

void DB_Interface::add_project(std::string project_name) {
  std::string add_project_st = "INSERT INTO projects (name)"
                               "VALUES ('" +
                               project_name + "');";
  auto rc = sqlite3_exec(db, add_project_st.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error adding project row.");
  }
}

void DB_Interface::edit_project_name(Id project_id,
                                     std::string new_project_name) {
  std::string alter_project_name =
    "UPDATE projects "
    "SET name = '" + new_project_name + "' " +
    "WHERE id = " + std::to_string(project_id) + ";";
  std::cerr << alter_project_name << std::endl;
  auto rc =
      sqlite3_exec(db, alter_project_name.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error renaming project.");
  }
}
