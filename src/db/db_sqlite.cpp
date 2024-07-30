#include "db_sqlite.h"
#include "db_sqlite_handle.h"

DB_SQLite::DB_SQLite(const std::filesystem::path &db_file)
  : sqlite_db (db_file) {
  this->create_projects_table();
  this->create_tasks_table();
}

std::vector<Project> DB_SQLite::query_projects() {
  std::string select_projects = "SELECT id, name FROM projects;";
  auto project_rows = sqlite_db.query_row_of_names(select_projects);
  auto projects = convert_namerows<Project>(project_rows);
  return projects;
}

std::vector<Task> DB_SQLite::query_tasks(Id project_id) {
  std::string select_tasks =
    "SELECT id, name "
    "FROM tasks "
    "WHERE project_id = " + std::to_string(project_id) + ";";
  auto task_rows = sqlite_db.query_row_of_names(select_tasks);
  auto tasks = convert_namerows<Task>(task_rows);
  return tasks;
}

void DB_SQLite::create_projects_table() {
  std::string create_projects_table_st =
    "CREATE TABLE  IF NOT EXISTS projects ("
    "id INTEGER PRIMARY KEY,"
    "name TEXT NOT NULL UNIQUE"
    ");";
  sqlite_db.exec_statement(create_projects_table_st);
}

void DB_SQLite::create_tasks_table() {
  std::string create_tasks_table_st =
      "CREATE TABLE IF NOT EXISTS tasks ("
      "id INTEGER PRIMARY KEY, "
      "name TEXT NOT NULL UNIQUE, "
      "project_id INTEGER, "
      "FOREIGN KEY (project_id) REFERENCES projects (id)"
      ");";
  sqlite_db.exec_statement(create_tasks_table_st);
}

void DB_SQLite::add_project(std::string project_name) {
  std::string add_project_st = "INSERT INTO projects (name)"
                               "VALUES ('" +
                               project_name + "');";
  try_exec_statement(add_project_st);
}

void DB_SQLite::add_task(Id project_id, std::string task_name) {
  std::string add_task_st = "INSERT INTO tasks (project_id, name) "
                            "VALUES ('" +
                            std::to_string(project_id) +
                            "', "
                            "'" +
                            task_name + "');";
  try_exec_statement(add_task_st);
}

void DB_SQLite::edit_project_name(Id project_id, std::string new_project_name) {
  std::string alter_project_name_st = "UPDATE projects "
                                      "SET name = '" +
                                      new_project_name +
                                      "' "
                                      "WHERE id = " +
                                      std::to_string(project_id) + ";";
  try_exec_statement(alter_project_name_st);
}

void DB_SQLite::edit_task_name(Id task_id, std::string new_task_name) {
  std::string alter_task_name_st =
    "UPDATE tasks "
    "SET name = '" +
    new_task_name +
    "' "
    "WHERE id = " + std::to_string(task_id) + ";";
  try_exec_statement(alter_task_name_st);
}

void DB_SQLite::try_exec_statement(const std::string &statement) {
  try {
    sqlite_db.exec_statement(statement);
  }
  catch (SQLiteConstraintExcept &e) {
    std::string msg = "DB logic error!\n";
    msg += e.what();
    throw DBLogicExcept(msg.c_str());
  }
}
