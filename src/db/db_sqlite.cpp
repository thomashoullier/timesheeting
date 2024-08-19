#include "db_sqlite.h"
#include "db_sqlite_handle.h"
#include <string>

DB_SQLite::DB_SQLite(const std::filesystem::path &db_file)
  : sqlite_db (db_file) {
  this->create_projects_table();
  this->create_tasks_table();
  this->create_entries_table();
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

std::vector<Entry> DB_SQLite::query_entries() {
  std::string select_entries =
    "SELECT e.id, p.name, t.name, e.start, e.stop "
    "FROM entries e "
    "INNER JOIN tasks t ON e.task_id = t.id "
    "INNER JOIN projects p ON t.project_id = p.id;";
  // TODO: put in the db_sqlite_handle somehow.
  auto stmt = sqlite_db.prepare_statement(select_entries);
  std::vector<Entry> vec;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(stmt, 0);
    std::string project_name = reinterpret_cast<const char*>
      (sqlite3_column_text(stmt, 1));
    std::string task_name = reinterpret_cast<const char*>
      (sqlite3_column_text(stmt, 2));
    uint64_t start_unix = sqlite3_column_int64(stmt, 3);
    uint64_t stop_unix = sqlite3_column_int64(stmt, 4);
    Date start_date(start_unix);
    Date stop_date(stop_unix);
    Entry e{id, project_name, task_name, start_date, stop_date};
    vec.push_back(e);
  }
  sqlite3_finalize(stmt);
  return vec;
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
      "name TEXT NOT NULL, "
      "project_id INTEGER, "
      "UNIQUE(name, project_id), "
      "FOREIGN KEY (project_id) REFERENCES projects (id) "
      "ON DELETE CASCADE"
      ");";
  sqlite_db.exec_statement(create_tasks_table_st);
}

void DB_SQLite::create_entries_table() {
  std::string create_entries_table_st =
    "CREATE TABLE IF NOT EXISTS entries ("
    "id INTEGER PRIMARY KEY, "
    "task_id INTEGER, "
    "start INTEGER, "
    "stop INTEGER, "
    "FOREIGN KEY (task_id) REFERENCES tasks (id) "
    ");";
  sqlite_db.exec_statement(create_entries_table_st);
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

void DB_SQLite::add_entry(Id task_id, const Date &start, const Date &stop) {
  std::string add_entry_st = "INSERT INTO entries (task_id, start, stop) "
    "VALUES ('" +
    std::to_string(task_id) + "', "
    "'" + std::to_string(start.to_unix_timestamp()) + "', "
    "'" + std::to_string(stop.to_unix_timestamp()) + "'"
    ");";
  try_exec_statement(add_entry_st);
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
  std::string alter_task_name_st = "UPDATE tasks "
                                   "SET name = '" +
                                   new_task_name +
                                   "' "
                                   "WHERE id = " +
                                   std::to_string(task_id) + ";";
  try_exec_statement(alter_task_name_st);
}

void DB_SQLite::edit_entry_start(Id entry_id, const Date &new_start_date) {
  std::string alter_entry_start_st = "UPDATE entries "
    "SET start = "
    "'" + std::to_string(new_start_date.to_unix_timestamp()) + "' "
    "WHERE id = " + std::to_string(entry_id) + ";";
  try_exec_statement(alter_entry_start_st);
  // TODO: ensure the DB consistency by also editing the stop date
  //       to equal the start date if start>stop.
  //       Be sure to use a single transaction for any changes!
}

void DB_SQLite::edit_entry_stop(Id entry_id, const Date &new_stop_date) {
  std::string alter_entry_stop_st =
      "UPDATE entries "
      "SET stop = "
      "'" +
      std::to_string(new_stop_date.to_unix_timestamp()) +
      "' "
      "WHERE id = " +
      std::to_string(entry_id) + ";";
  try_exec_statement(alter_entry_stop_st);
}

void DB_SQLite::delete_task(Id task_id) {
  std::string delete_task_st =
    "DELETE FROM tasks "
    "WHERE id = " + std::to_string(task_id) + ";";
  try_exec_statement(delete_task_st);
}

void DB_SQLite::delete_project(Id project_id) {
  std::string delete_project_st =
    "DELETE FROM projects "
    "WHERE id = " + std::to_string(project_id) + ";";
  try_exec_statement(delete_project_st);
}

void DB_SQLite::try_exec_statement(const std::string &statement) {
  try {
    sqlite_db.exec_statement(statement);
  } catch (SQLiteConstraintExcept &e) {
    std::string msg = "DB logic error!\n";
    msg += e.what();
    throw DBLogicExcept(msg.c_str());
  }
}
