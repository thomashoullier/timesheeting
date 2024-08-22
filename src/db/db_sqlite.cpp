#include "db_sqlite.h"
#include "db_sqlite_handle.h"
#include <iostream>
#include <optional>
#include <string>

DB_SQLite::DB_SQLite(const std::filesystem::path &db_file)
  : sqlite_db (db_file) {
  this->create_projects_table();
  this->create_tasks_table();
  this->create_entries_table();
  this->create_entrystaging_table();
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

std::vector<Entry> DB_SQLite::query_entries(const DateRange &date_range) {
  auto start_stamp = date_range.start.to_unix_timestamp();
  auto stop_stamp = date_range.stop.to_unix_timestamp();
  std::string select_entries =
    "SELECT e.id, p.name, t.name, e.start, e.stop "
    "FROM entries e "
    "INNER JOIN tasks t ON e.task_id = t.id "
    "INNER JOIN projects p ON t.project_id = p.id "
    "WHERE e.start >= " + std::to_string(start_stamp) + " "
    "AND e.start < " + std::to_string(stop_stamp) + " "
    "ORDER BY e.start ASC;";
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

EntryStaging DB_SQLite::query_entrystaging() {
  std::string select_entrystaging_st =
    "SELECT p.name, t.name, start, stop "
    "FROM entrystaging "
    "LEFT JOIN tasks t ON entrystaging.task_id = t.id "
    "LEFT JOIN projects p ON t.project_id = p.id "
    ";";
  auto stmt = sqlite_db.prepare_statement(select_entrystaging_st);
  if (!(sqlite3_step(stmt) == SQLITE_ROW))
    throw DBLogicExcept("query_entrystaging: could not read SQLITE_ROW");

  std::optional<std::string> project_name;
  auto project_name_ret = reinterpret_cast<const char*>
    (sqlite3_column_text(stmt, 0));
  if (project_name_ret == NULL)
    project_name = std::nullopt;
  else
    project_name = project_name_ret;

  std::optional<std::string> task_name;
  auto task_name_ret = reinterpret_cast<const char*>
    (sqlite3_column_text(stmt, 1));
  if (task_name_ret == NULL)
    task_name = std::nullopt;
  else
    task_name = task_name_ret;

  std::optional<Date> start_date;
  uint64_t start_unix = sqlite3_column_int64(stmt, 2);
  if (start_unix == 0)
    start_date = std::nullopt;
  else
    start_date = Date(start_unix);

  std::optional<Date> stop_date;
  uint64_t stop_unix = sqlite3_column_int64(stmt, 3);
  if (stop_unix == 0)
    stop_date = std::nullopt;
  else
    stop_date = Date(stop_unix);
  sqlite3_finalize(stmt);

  return EntryStaging{project_name, task_name, start_date, stop_date};
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
      "project_id INTEGER NOT NULL, "
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
    "task_id INTEGER NOT NULL, "
    "start INTEGER NOT NULL, "
    "stop INTEGER NOT NULL, "
    "FOREIGN KEY (task_id) REFERENCES tasks (id), "
    "CHECK (start <= stop) "
    ");";
  sqlite_db.exec_statement(create_entries_table_st);
}

void DB_SQLite::create_entrystaging_table() {
  std::string create_entrystaging_table_st =
    "CREATE TABLE IF NOT EXISTS entrystaging ("
    "id INTEGER PRIMARY KEY CHECK (id = 1), "
    "task_id INTEGER, "
    "start INTEGER, "
    "stop INTEGER, "
    "FOREIGN KEY (task_id) REFERENCES tasks (id) "
    ");" ;
  sqlite_db.exec_statement(create_entrystaging_table_st);

  // Insert a first row if it does not already exist.
  std::string insert_row_st =
    "INSERT INTO entrystaging (task_id, start, stop) "
    "SELECT NULL, NULL, NULL "
    "WHERE NOT EXISTS (SELECT * FROM entrystaging)"
    ";";
  try_exec_statement(insert_row_st);
}

void DB_SQLite::add_project(std::string project_name) {
  std::string add_project_st = "INSERT INTO projects (name) "
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

void DB_SQLite::edit_entry_project(Id entry_id,
                                   const std::string &new_project_name) {
  std::string alter_entry_project_st =
    "UPDATE entries "
    "SET task_id = ("
    "SELECT tasks.id FROM tasks "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "WHERE projects.name = '" + new_project_name + "'"
    ") "
    "WHERE entries.id = " + std::to_string(entry_id) + " "
    "LIMIT 1;";
  try_exec_statement(alter_entry_project_st);
}

void DB_SQLite::edit_entry_task(Id entry_id, const std::string &new_task_name) {
  std::string alter_entry_task_st =
    "UPDATE entries "
    "SET task_id = ("
    "SELECT id FROM tasks "
    "WHERE name = '" + new_task_name + "' "
    "AND project_id = ("
    "SELECT projects.id FROM entries "
    "INNER JOIN tasks ON entries.task_id = tasks.id "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "WHERE entries.id = " + std::to_string(entry_id) +
    ")"
    ") "
    "WHERE entries.id = " + std::to_string(entry_id) + ";";
  try_exec_statement(alter_entry_task_st);
}

void DB_SQLite::edit_entry_start(Id entry_id, const Date &new_start_date) {
  std::string alter_entry_start_st = "UPDATE entries "
    "SET start = "
    "'" + std::to_string(new_start_date.to_unix_timestamp()) + "' "
    "WHERE id = " + std::to_string(entry_id) + ";";
  try_exec_statement(alter_entry_start_st);
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

void DB_SQLite::edit_entrystaging_project_name
    (const std::string &new_project_name) {
  std::string alter_entrystaging_project_st =
    "UPDATE entrystaging "
    "SET task_id = ("
    "SELECT tasks.id FROM tasks "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "WHERE projects.name = '" + new_project_name + "'"
    ") "
    "LIMIT 1;";
  try_exec_statement(alter_entrystaging_project_st);
}

void DB_SQLite::edit_entrystaging_task_name(const std::string &new_task_name) {
  std::string alter_entrystaging_task_st =
    "UPDATE entrystaging "
    "SET task_id = ("
    "SELECT id FROM tasks "
    "WHERE name = '" + new_task_name + "' "
    "AND project_id = ("
    "SELECT projects.id FROM entrystaging "
    "INNER JOIN tasks ON entrystaging.task_id = tasks.id "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "));";
  try_exec_statement(alter_entrystaging_task_st);
}

void DB_SQLite::edit_entrystaging_start(const Date &new_start) {
  std::string alter_entrystaging_start_st =
      "UPDATE entrystaging "
      "SET start = "
      "'" + std::to_string(new_start.to_unix_timestamp()) + "';";
  try_exec_statement(alter_entrystaging_start_st);
}
void DB_SQLite::edit_entrystaging_stop(const Date &new_stop) {
  std::string alter_entrystaging_stop_st =
      "UPDATE entrystaging "
      "SET stop = "
      "'" + std::to_string(new_stop.to_unix_timestamp()) + "';";
  try_exec_statement(alter_entrystaging_stop_st);
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

void DB_SQLite::delete_entry(Id entry_id) {
  std::string delete_entry_st =
    "DELETE FROM entries "
    "WHERE id = " + std::to_string(entry_id) + ";";
  try_exec_statement(delete_entry_st);
}

void DB_SQLite::commit_entrystaging(){
  std::string commit_entrystaging_st =
    "INSERT INTO entries (task_id, start, stop) "
    "SELECT task_id, start, stop "
    "FROM entrystaging;";
  try_exec_statement(commit_entrystaging_st);
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
