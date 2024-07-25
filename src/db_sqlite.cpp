#include "db_sqlite.h"

DB_SQLite::DB_SQLite (std::string db_file) {
  auto rc = sqlite3_open(db_file.c_str(), &db);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Cannot open database file.");
  }
  this->create_projects_table();
  this->create_tasks_table();
}

DB_SQLite::~DB_SQLite() { sqlite3_close(db); }

std::vector<Project> DB_SQLite::query_projects() {
  std::string select_projects = "SELECT id, name FROM projects;";
  sqlite3_stmt *stmt;
  auto rc = sqlite3_prepare_v2(db, select_projects.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Could not prepare SQL statement.");
  }
  // Get and parse each of the results in the rows.
  std::vector<Project> projects{};
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Project proj{};
    proj.id = sqlite3_column_int64(stmt, 0);
    auto name = sqlite3_column_text(stmt, 1);
    proj.name = reinterpret_cast<const char *>(name);
    projects.push_back(proj);
  }
  sqlite3_finalize(stmt);
  return projects;
}

std::vector<Task> DB_SQLite::query_tasks(Id project_id) {
  std::string select_tasks =
    "SELECT id, name "
    "FROM tasks "
    "WHERE project_id = " + std::to_string(project_id) + ";";
  sqlite3_stmt *stmt;
  auto rc = sqlite3_prepare_v2(db, select_tasks.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Could not prepare SQL statement.");
  }
  std::vector<Task> tasks{};
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Task task{};
    task.id = sqlite3_column_int64(stmt, 0);
    auto name = sqlite3_column_text(stmt, 1);
    task.name = reinterpret_cast<const char *>(name);
    tasks.push_back(task);
  }
  sqlite3_finalize(stmt);
  return tasks;
}

void DB_SQLite::create_projects_table() {
  std::string create_projects_table_st =
    "CREATE TABLE  IF NOT EXISTS projects ("
    "id INTEGER PRIMARY KEY,"
    "name TEXT NOT NULL"
    ");";
  auto rc = sqlite3_exec(db, create_projects_table_st.c_str(),
                         NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error creating projects table.");
  }
}

void DB_SQLite::create_tasks_table() {
  std::string create_tasks_table_st =
      "CREATE TABLE IF NOT EXISTS tasks ("
      "id INTEGER PRIMARY KEY, "
      "name TEXT NOT NULL, "
      "project_id INTEGER, "
      "FOREIGN KEY (project_id) REFERENCES projects (id)"
      ");";
  auto rc =
      sqlite3_exec(db, create_tasks_table_st.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error creating tasks table.");
  }
}

void DB_SQLite::add_project(std::string project_name) {
  std::string add_project_st = "INSERT INTO projects (name)"
                               "VALUES ('" +
                               project_name + "');";
  auto rc = sqlite3_exec(db, add_project_st.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error adding project row.");
  }
}

void DB_SQLite::add_task(Id project_id, std::string task_name) {
  std::string add_task_st = "INSERT INTO tasks (project_id, name) "
                            "VALUES ('" +
                            std::to_string(project_id) +
                            "', "
                            "'" +
                            task_name + "');";
  auto rc = sqlite3_exec(db, add_task_st.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error adding task row.");
  }
}

void DB_SQLite::edit_project_name(Id project_id,
                                     std::string new_project_name) {
  std::string alter_project_name = "UPDATE projects "
                                   "SET name = '" +
                                   new_project_name + "' " +
                                   "WHERE id = " + std::to_string(project_id) +
                                   ";";
  auto rc = sqlite3_exec(db, alter_project_name.c_str(), NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error renaming project.");
  }
}
