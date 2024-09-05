#include "db_sqlite.h"
#include "db_sqlite_handle.h"
#include <cstdint>
#include <optional>
#include <string>
#include "../logger/logger.h"

DB_SQLite::DB_SQLite(const std::filesystem::path &db_file)
  : sqlite_db (db_file) {
  this->create_projects_table();
  this->create_tasks_table();
  this->create_locations_table();
  this->create_entries_table();
  this->create_entries_start_index();
  this->create_entrystaging_table();

  // Prepare persistent statements
  std::string select_projects_st = "SELECT id, name FROM projects;";
  select_projects = sqlite_db.prepare_statement(select_projects_st);

  std::string select_projects_active_st =
    "SELECT id, name FROM projects WHERE active = TRUE;";
  select_projects_active =
    sqlite_db.prepare_statement(select_projects_active_st);

  std::string select_tasks_st = "SELECT id, name "
                                "FROM tasks "
                                "WHERE project_id = ? ;";
  select_tasks = sqlite_db.prepare_statement(select_tasks_st);

  std::string select_tasks_active_st =
    "SELECT id, name "
    "FROM tasks "
    "WHERE project_id = ? "
    "AND active = TRUE;";
  select_tasks_active = sqlite_db.prepare_statement(select_tasks_active_st);

  std::string select_locations_st = "SELECT id, name FROM locations;";
  select_locations = sqlite_db.prepare_statement(select_locations_st);

  std::string select_locations_active_st =
    "SELECT id, name FROM locations "
    "WHERE active = TRUE;";
  select_locations_active = sqlite_db.prepare_statement
    (select_locations_active_st);

  std::string select_entries_st =
    "SELECT e.id, p.name, t.name, e.start, e.stop, l.name "
    "FROM entries e "
    "INNER JOIN locations l ON e.location_id = l.id "
    "INNER JOIN tasks t ON e.task_id = t.id "
    "INNER JOIN projects p ON t.project_id = p.id "
    "WHERE e.start >= ? "
    "AND e.start < ? "
    "ORDER BY e.start ASC;";
  select_entries = sqlite_db.prepare_statement(select_entries_st);

  std::string select_duration_st =
    "SELECT SUM(stop - start) FROM entries e "
    "WHERE e.start >= ? "
    "AND e.start < ? ;";
  select_duration = sqlite_db.prepare_statement(select_duration_st);

  std::string select_entrystaging_st =
    "SELECT p.name, t.name, start, stop, l.name "
    "FROM entrystaging "
    "LEFT JOIN locations l ON entrystaging.location_id = l.id "
    "LEFT JOIN tasks t ON entrystaging.task_id = t.id "
    "LEFT JOIN projects p ON t.project_id = p.id "
    ";";
  select_entrystaging = sqlite_db.prepare_statement(select_entrystaging_st);

  std::string insert_project_st =
    "INSERT INTO projects (name, active) "
    "VALUES (?, TRUE);";
  insert_project = sqlite_db.prepare_statement(insert_project_st);

  std::string insert_task_st =
    "INSERT INTO tasks (project_id, name, active) "
    "VALUES (?, ?, TRUE);";
  insert_task = sqlite_db.prepare_statement(insert_task_st);

  std::string insert_location_st =
    "INSERT INTO locations (name, active) "
    "VALUES (?, 1);";
  insert_location = sqlite_db.prepare_statement(insert_location_st);

  std::string insert_entry_st =
    "INSERT INTO entries (task_id, start, stop, location_id) "
    "VALUES (?, ?, ?, ?);";
  insert_entry = sqlite_db.prepare_statement(insert_entry_st);

  std::string update_project_name_st =
    "UPDATE projects "
    "SET name = ? WHERE id = ?;";
  update_project_name = sqlite_db.prepare_statement(update_project_name_st);

  std::string alter_task_name_st = "UPDATE tasks SET name = ? WHERE id = ?;";
  update_task_name = sqlite_db.prepare_statement(alter_task_name_st);

  std::string alter_location_name_st =
    "UPDATE locations SET name = ? WHERE id = ?;";
  update_location_name = sqlite_db.prepare_statement(alter_location_name_st);

  std::string toggle_location_flag_st =
    "UPDATE locations SET active = NOT active "
    "WHERE id = ?;";
  toggle_location_flag = sqlite_db.prepare_statement(toggle_location_flag_st);

  std::string toggle_task_flag_st =
    "UPDATE tasks SET active = NOT active "
    "WHERE id = ?;";
  toggle_task_flag = sqlite_db.prepare_statement(toggle_task_flag_st);

  std::string toggle_project_flag_st =
    "UPDATE projects SET active = NOT active "
    "WHERE id = ?";
  toggle_project_flag = sqlite_db.prepare_statement(toggle_project_flag_st);

  std::string update_entry_project_st =
    "UPDATE entries "
    "SET task_id = ("
    "SELECT tasks.id FROM tasks "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "WHERE projects.name = ?) "
    "WHERE entries.id = ? LIMIT 1;";
  update_entry_project = sqlite_db.prepare_statement(update_entry_project_st);

  std::string update_entry_task_st =
    "UPDATE entries "
    "SET task_id = ("
    "SELECT id FROM tasks "
    "WHERE name = ?1 "
    "AND project_id = ("
    "SELECT projects.id FROM entries "
    "INNER JOIN tasks ON entries.task_id = tasks.id "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "WHERE entries.id = ?2)) "
    "WHERE entries.id = ?2;";
  update_entry_task = sqlite_db.prepare_statement(update_entry_task_st);

  std::string update_entry_start_st =
    "UPDATE entries SET start = ? WHERE id = ?;";
  update_entry_start = sqlite_db.prepare_statement(update_entry_start_st);

  std::string update_entry_stop_st =
      "UPDATE entries SET stop = ? WHERE id = ?;";
  update_entry_stop = sqlite_db.prepare_statement(update_entry_stop_st);

  std::string update_entry_location_st =
    "UPDATE entries SET location_id = ("
    "SELECT id FROM locations "
    "WHERE name = ?) "
    "WHERE id = ?;";
  update_entry_location = sqlite_db.prepare_statement(update_entry_location_st);

  std::string update_entrystaging_project_st =
    "UPDATE entrystaging "
    "SET task_id = ("
    "SELECT tasks.id FROM tasks "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "WHERE projects.name = ? "
    "AND projects.active = TRUE "
    "AND tasks.active = TRUE"
    ") LIMIT 1;";
  update_entrystaging_project =
    sqlite_db.prepare_statement(update_entrystaging_project_st);

  std::string update_entrystaging_task_st =
    "UPDATE entrystaging "
    "SET task_id = ("
    "SELECT id FROM tasks "
    "WHERE name = ? "
    "AND active = TRUE "
    "AND project_id = ("
    "SELECT projects.id FROM entrystaging "
    "INNER JOIN tasks ON entrystaging.task_id = tasks.id "
    "INNER JOIN projects ON tasks.project_id = projects.id "
    "));";
  update_entrystaging_task =
    sqlite_db.prepare_statement(update_entrystaging_task_st);

  std::string update_entrystaging_start_st =
    "UPDATE entrystaging SET start = ?;";
  update_entrystaging_start =
    sqlite_db.prepare_statement(update_entrystaging_start_st);

  std::string update_entrystaging_stop_st = "UPDATE entrystaging SET stop = ?;";
  update_entrystaging_stop =
    sqlite_db.prepare_statement(update_entrystaging_stop_st);

  std::string update_entrystaging_location_st =
    "UPDATE entrystaging SET location_id = ("
    "SELECT id FROM locations "
    "WHERE name = ? "
    "AND active = TRUE);";
  update_entrystaging_location =
    sqlite_db.prepare_statement(update_entrystaging_location_st);

  std::string remove_task_st = "DELETE FROM tasks WHERE id = ?;";
  remove_task = sqlite_db.prepare_statement(remove_task_st);

  std::string remove_project_st = "DELETE FROM projects WHERE id = ?;";
  remove_project = sqlite_db.prepare_statement(remove_project_st);

  std::string remove_location_st = "DELETE FROM locations WHERE id = ?;";
  remove_location = sqlite_db.prepare_statement(remove_location_st);

  std::string remove_entry_st = "DELETE FROM entries WHERE id = ?;";
  remove_entry = sqlite_db.prepare_statement(remove_entry_st);

  std::string insert_entrystaging_st =
    "INSERT INTO entries (task_id, start, stop, location_id) "
    "SELECT task_id, start, stop, location_id "
    "FROM entrystaging;";
  insert_entrystaging = sqlite_db.prepare_statement(insert_entrystaging_st);

  std::string sum_duration_per_project_st =
    "SELECT projects.name, SUM(entries.stop - entries.start) "
    "FROM projects "
    "INNER JOIN tasks ON projects.id = tasks.project_id "
    "INNER JOIN entries ON tasks.id = entries.task_id "
    "WHERE entries.start >= ? "
    "AND entries.start < ? "
    "GROUP BY projects.name;";
  sum_duration_per_project =
    sqlite_db.prepare_statement(sum_duration_per_project_st);
}

DB_SQLite::~DB_SQLite() {
  sqlite3_finalize(select_projects);
  sqlite3_finalize(select_projects_active);
  sqlite3_finalize(select_tasks);
  sqlite3_finalize(select_tasks_active);
  sqlite3_finalize(select_locations);
  sqlite3_finalize(select_locations_active);
  sqlite3_finalize(select_entries);
  sqlite3_finalize(select_duration);
  sqlite3_finalize(select_entrystaging);
  sqlite3_finalize(insert_project);
  sqlite3_finalize(insert_task);
  sqlite3_finalize(insert_location);
  sqlite3_finalize(insert_entry);
  sqlite3_finalize(update_project_name);
  sqlite3_finalize(update_task_name);
  sqlite3_finalize(update_location_name);
  sqlite3_finalize(toggle_location_flag);
  sqlite3_finalize(toggle_task_flag);
  sqlite3_finalize(toggle_project_flag);
  sqlite3_finalize(update_entry_project);
  sqlite3_finalize(update_entry_task);
  sqlite3_finalize(update_entry_start);
  sqlite3_finalize(update_entry_stop);
  sqlite3_finalize(update_entry_location);
  sqlite3_finalize(update_entrystaging_project);
  sqlite3_finalize(update_entrystaging_task);
  sqlite3_finalize(update_entrystaging_start);
  sqlite3_finalize(update_entrystaging_stop);
  sqlite3_finalize(update_entrystaging_location);
  sqlite3_finalize(remove_task);
  sqlite3_finalize(remove_project);
  sqlite3_finalize(remove_location);
  sqlite3_finalize(remove_entry);
  sqlite3_finalize(insert_entrystaging);
  sqlite3_finalize(sum_duration_per_project);
}

std::vector<Project> DB_SQLite::query_projects() {
  sqlite3_reset(select_projects);
  NameRows rows{};
  while (sqlite3_step(select_projects) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(select_projects, 0);
    auto name_internal = sqlite3_column_text(select_projects, 1);
    std::string name = reinterpret_cast<const char *>(name_internal);
    rows.push_back(std::make_pair(id, name));
  }
  auto projects = convert_namerows<Project>(rows);
  return projects;
}

std::vector<Project> DB_SQLite::query_projects_active() {
  sqlite3_reset(select_projects_active);
  NameRows rows{};
  while (sqlite3_step(select_projects_active) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(select_projects_active, 0);
    auto name_internal = sqlite3_column_text(select_projects_active, 1);
    std::string name = reinterpret_cast<const char *>(name_internal);
    rows.push_back(std::make_pair(id, name));
  }
  auto projects = convert_namerows<Project>(rows);
  return projects;
}

std::vector<Task> DB_SQLite::query_tasks(Id project_id) {
  sqlite3_reset(select_tasks);
  sqlite3_bind_int64(select_tasks, 1, project_id);
  NameRows rows{};
  while (sqlite3_step(select_tasks) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(select_tasks, 0);
    auto name_internal = sqlite3_column_text(select_tasks, 1);
    std::string name = reinterpret_cast<const char *>(name_internal);
    rows.push_back(std::make_pair(id, name));
  }
  auto tasks = convert_namerows<Task>(rows);
  return tasks;
}

std::vector<Task> DB_SQLite::query_tasks_active(Id project_id) {
  sqlite3_reset(select_tasks_active);
  sqlite3_bind_int64(select_tasks_active, 1, project_id);
  NameRows rows{};
  while (sqlite3_step(select_tasks_active) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(select_tasks_active, 0);
    auto name_internal = sqlite3_column_text(select_tasks_active, 1);
    std::string name = reinterpret_cast<const char *>(name_internal);
    rows.push_back(std::make_pair(id, name));
  }
  auto tasks = convert_namerows<Task>(rows);
  return tasks;
}

std::vector<Location> DB_SQLite::query_locations() {
  sqlite3_reset(select_locations);
  NameRows rows{};
  while (sqlite3_step(select_locations) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(select_locations, 0);
    auto name_internal = sqlite3_column_text(select_locations, 1);
    std::string name = reinterpret_cast<const char *>(name_internal);
    rows.push_back(std::make_pair(id, name));
  }
  auto locations = convert_namerows<Location>(rows);
  return locations;
}

std::vector<Location> DB_SQLite::query_locations_active() {
  sqlite3_reset(select_locations_active);
  NameRows rows{};
  while (sqlite3_step(select_locations_active) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(select_locations_active, 0);
    auto name_internal = sqlite3_column_text(select_locations_active, 1);
    std::string name = reinterpret_cast<const char *>(name_internal);
    rows.push_back(std::make_pair(id, name));
  }
  auto locations = convert_namerows<Location>(rows);
  return locations;
}

std::vector<Entry> DB_SQLite::query_entries(const DateRange &date_range) {
  uint64_t start_stamp = date_range.start.to_unix_timestamp();
  uint64_t stop_stamp = date_range.stop.to_unix_timestamp();
  sqlite3_reset(select_entries);
  sqlite3_bind_int64(select_entries, 1, start_stamp);
  sqlite3_bind_int64(select_entries, 2, stop_stamp);
  std::vector<Entry> vec;
  while (sqlite3_step(select_entries) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(select_entries, 0);
    std::string project_name = reinterpret_cast<const char*>
      (sqlite3_column_text(select_entries, 1));
    std::string task_name = reinterpret_cast<const char*>
      (sqlite3_column_text(select_entries, 2));
    uint64_t start_unix = sqlite3_column_int64(select_entries, 3);
    uint64_t stop_unix = sqlite3_column_int64(select_entries, 4);
    Date start_date(start_unix);
    Date stop_date(stop_unix);
    std::string location_name = reinterpret_cast<const char*>
      (sqlite3_column_text(select_entries, 5));
    Entry e{id, project_name, task_name, start_date, stop_date,
            location_name};
    vec.push_back(e);
  }
  return vec;
}

Duration DB_SQLite::query_entries_duration(const DateRange &date_range) {
  uint64_t start_stamp = date_range.start.to_unix_timestamp();
  uint64_t stop_stamp = date_range.stop.to_unix_timestamp();
  sqlite3_reset(select_duration);
  sqlite3_bind_int64(select_duration, 1, start_stamp);
  sqlite3_bind_int64(select_duration, 2, stop_stamp);
  Duration total = (sqlite3_step(select_duration) == SQLITE_ROW) ?
    Duration(sqlite3_column_int64(select_duration, 0)) : Duration();
  return total;
}

EntryStaging DB_SQLite::query_entrystaging() {
  sqlite3_reset(select_entrystaging);
  if (!(sqlite3_step(select_entrystaging) == SQLITE_ROW))
    throw DBLogicExcept("query_entrystaging: could not read SQLITE_ROW");

  std::optional<std::string> project_name;
  auto project_name_ret = reinterpret_cast<const char*>
    (sqlite3_column_text(select_entrystaging, 0));
  if (project_name_ret == NULL)
    project_name = std::nullopt;
  else
    project_name = project_name_ret;

  std::optional<std::string> task_name;
  auto task_name_ret = reinterpret_cast<const char*>
    (sqlite3_column_text(select_entrystaging, 1));
  if (task_name_ret == NULL)
    task_name = std::nullopt;
  else
    task_name = task_name_ret;

  std::optional<Date> start_date;
  uint64_t start_unix = sqlite3_column_int64(select_entrystaging, 2);
  if (start_unix == 0)
    start_date = std::nullopt;
  else
    start_date = Date(start_unix);

  std::optional<Date> stop_date;
  uint64_t stop_unix = sqlite3_column_int64(select_entrystaging, 3);
  if (stop_unix == 0)
    stop_date = std::nullopt;
  else
    stop_date = Date(stop_unix);

  std::optional<std::string> location_name;
  auto location_name_ret = reinterpret_cast<const char*>
    (sqlite3_column_text(select_entrystaging, 4));
  if (location_name_ret == NULL)
    location_name = std::nullopt;
  else
    location_name = location_name_ret;

  return EntryStaging{project_name, task_name, start_date, stop_date,
                      location_name};
}

void DB_SQLite::create_projects_table() {
  std::string create_projects_table_st =
    "CREATE TABLE  IF NOT EXISTS projects ("
    "id INTEGER PRIMARY KEY,"
    "name TEXT NOT NULL UNIQUE, "
    "active BOOLEAN NOT NULL CHECK (active IN (0,1))"
    ");";
  sqlite_db.exec_statement(create_projects_table_st);
}

void DB_SQLite::create_tasks_table() {
  std::string create_tasks_table_st =
      "CREATE TABLE IF NOT EXISTS tasks ("
      "id INTEGER PRIMARY KEY, "
      "name TEXT NOT NULL, "
      "project_id INTEGER NOT NULL, "
      "active BOOLEAN NOT NULL CHECK (active IN (0,1)), "
      "UNIQUE(name, project_id), "
      "FOREIGN KEY (project_id) REFERENCES projects (id) "
      "ON DELETE CASCADE"
      ");";
  sqlite_db.exec_statement(create_tasks_table_st);
}

void DB_SQLite::create_locations_table() {
  std::string create_locations_table_st =
    "CREATE TABLE IF NOT EXISTS locations ("
    "id INTEGER PRIMARY KEY, "
    "name TEXT NOT NULL UNIQUE, "
    "active BOOLEAN NOT NULL CHECK (active IN (0,1))"
    ");";
  sqlite_db.exec_statement(create_locations_table_st);
}

void DB_SQLite::create_entries_table() {
  std::string create_entries_table_st =
    "CREATE TABLE IF NOT EXISTS entries ("
    "id INTEGER PRIMARY KEY, "
    "task_id INTEGER NOT NULL, "
    "start INTEGER NOT NULL UNIQUE, "
    "stop INTEGER NOT NULL, "
    "location_id INTEGER NOT NULL, "
    "FOREIGN KEY (task_id) REFERENCES tasks (id), "
    "FOREIGN KEY (location_id) REFERENCES locations (id), "
    "CHECK (start <= stop) "
    ");";
  sqlite_db.exec_statement(create_entries_table_st);
}

void DB_SQLite::create_entries_start_index() {
  std::string create_entries_start_index_st =
    "CREATE UNIQUE INDEX IF NOT EXISTS idx_entries_start "
    "ON entries (start);";
  sqlite_db.exec_statement(create_entries_start_index_st);
}

void DB_SQLite::create_entrystaging_table() {
  std::string create_entrystaging_table_st =
    "CREATE TABLE IF NOT EXISTS entrystaging ("
    "id INTEGER PRIMARY KEY CHECK (id = 1), "
    "task_id INTEGER, "
    "start INTEGER, "
    "stop INTEGER, "
    "location_id INTEGER, "
    "FOREIGN KEY (task_id) REFERENCES tasks (id), "
    "FOREIGN KEY (location_id) REFERENCES locations (id)"
    ");";
  sqlite_db.exec_statement(create_entrystaging_table_st);

  // Insert a first row if it does not already exist.
  std::string insert_row_st =
    "INSERT INTO entrystaging (task_id, start, stop, location_id) "
    "SELECT NULL, NULL, NULL, NULL "
    "WHERE NOT EXISTS (SELECT * FROM entrystaging)"
    ";";
  try_exec_statement(insert_row_st);
}

void DB_SQLite::add_project(std::string project_name) {
  sqlite3_reset(insert_project);
  sqlite3_bind_text(insert_project, 1,
                    project_name.c_str(), project_name.size(),
                    SQLITE_STATIC);
  try_step_statement(insert_project);
}

void DB_SQLite::add_task(Id project_id, std::string task_name) {
  sqlite3_reset(insert_task);
  sqlite3_bind_int64(insert_task, 1, project_id);
  sqlite3_bind_text(insert_task, 2, task_name.c_str(), task_name.size(),
                    SQLITE_STATIC);
  try_step_statement(insert_task);
}

void DB_SQLite::add_location(const std::string &location_name) {
  sqlite3_reset(insert_location);
  sqlite3_bind_text(insert_location, 1,
                    location_name.c_str(), location_name.size(),
                    SQLITE_STATIC);
  try_step_statement(insert_location);
}

void DB_SQLite::add_entry(Id task_id, const Date &start, const Date &stop,
                          Id location_id) {
  sqlite3_reset(insert_entry);
  sqlite3_bind_int64(insert_entry, 1, task_id);
  sqlite3_bind_int64(insert_entry, 2, start.to_unix_timestamp());
  sqlite3_bind_int64(insert_entry, 3, stop.to_unix_timestamp());
  sqlite3_bind_int64(insert_entry, 4, location_id);
  try_step_statement(insert_entry);
}

void DB_SQLite::edit_project_name(Id project_id, std::string new_project_name) {
  sqlite3_reset(update_project_name);
  sqlite3_bind_text(update_project_name, 1,
                    new_project_name.c_str(), new_project_name.size(),
                    SQLITE_STATIC);
  sqlite3_bind_int64(update_project_name, 2, project_id);
  try_step_statement(update_project_name);
}

void DB_SQLite::edit_location_name(Id location_id,
                                   const std::string &new_location_name) {
  sqlite3_reset(update_location_name);
  sqlite3_bind_text(update_location_name, 1,
                    new_location_name.c_str(), new_location_name.size(),
                    SQLITE_STATIC);
  sqlite3_bind_int64(update_location_name, 2, location_id);
  try_step_statement(update_location_name);
}

void DB_SQLite::toggle_location_active(Id location_id) {
  sqlite3_reset(toggle_location_flag);
  sqlite3_bind_int64(toggle_location_flag, 1, location_id);
  try_step_statement(toggle_location_flag);
}

void DB_SQLite::toggle_task_active(Id task_id) {
  sqlite3_reset(toggle_task_flag);
  sqlite3_bind_int64(toggle_task_flag, 1, task_id);
  try_step_statement(toggle_task_flag);
}

void DB_SQLite::toggle_project_active(Id project_id) {
  sqlite3_reset(toggle_project_flag);
  sqlite3_bind_int64(toggle_project_flag, 1, project_id);
  try_step_statement(toggle_project_flag);
}

void DB_SQLite::edit_task_name(Id task_id, std::string new_task_name) {
  sqlite3_reset(update_task_name);
  sqlite3_bind_text(update_task_name, 1,
                    new_task_name.c_str(), new_task_name.size(),
                    SQLITE_STATIC);
  sqlite3_bind_int64(update_task_name, 2, task_id);
  try_step_statement(update_task_name);
}

void DB_SQLite::edit_entry_project(Id entry_id,
                                   const std::string &new_project_name) {
  sqlite3_reset(update_entry_project);
  sqlite3_bind_text(update_entry_project, 1, new_project_name.c_str(),
                    new_project_name.size(), SQLITE_STATIC);
  sqlite3_bind_int64(update_entry_project, 2, entry_id);
  try_step_statement(update_entry_project);
}

void DB_SQLite::edit_entry_task(Id entry_id, const std::string &new_task_name) {
  sqlite3_reset(update_entry_task);
  sqlite3_bind_text(update_entry_task, 1, new_task_name.c_str(),
                    new_task_name.size(), SQLITE_STATIC);
  sqlite3_bind_int64(update_entry_task, 2, entry_id);
  try_step_statement(update_entry_task);
}

void DB_SQLite::edit_entry_start(Id entry_id, const Date &new_start_date) {
  sqlite3_reset(update_entry_start);
  sqlite3_bind_int64(update_entry_start, 1, new_start_date.to_unix_timestamp());
  sqlite3_bind_int64(update_entry_start, 2, entry_id);
  try_step_statement(update_entry_start);
}

void DB_SQLite::edit_entry_stop(Id entry_id, const Date &new_stop_date) {
  sqlite3_reset(update_entry_stop);
  sqlite3_bind_int64(update_entry_stop, 1, new_stop_date.to_unix_timestamp());
  sqlite3_bind_int64(update_entry_stop, 2, entry_id);
  try_step_statement(update_entry_stop);
}

void DB_SQLite::edit_entry_location(Id entry_id,
                                    const std::string &new_location_name) {
  sqlite3_reset(update_entry_location);
  sqlite3_bind_text(update_entry_location, 1, new_location_name.c_str(),
                    new_location_name.size(), SQLITE_STATIC);
  sqlite3_bind_int64(update_entry_location, 2, entry_id);
  try_step_statement(update_entry_location);
}

void DB_SQLite::edit_entrystaging_project_name
    (const std::string &new_project_name) {
  sqlite3_reset(update_entrystaging_project);
  sqlite3_bind_text(update_entrystaging_project, 1, new_project_name.c_str(),
                    new_project_name.size(), SQLITE_STATIC);
  try_step_statement(update_entrystaging_project);
}

void DB_SQLite::edit_entrystaging_task_name(const std::string &new_task_name) {
  sqlite3_reset(update_entrystaging_task);
  sqlite3_bind_text(update_entrystaging_task, 1, new_task_name.c_str(),
                    new_task_name.size(), SQLITE_STATIC);
  try_step_statement(update_entrystaging_task);
}

void DB_SQLite::edit_entrystaging_start(const Date &new_start) {
  sqlite3_reset(update_entrystaging_start);
  sqlite3_bind_int64(update_entrystaging_start, 1,
                     new_start.to_unix_timestamp());
  try_step_statement(update_entrystaging_start);
}

void DB_SQLite::edit_entrystaging_stop(const Date &new_stop) {
  sqlite3_reset(update_entrystaging_stop);
  sqlite3_bind_int64(update_entrystaging_stop, 1,
                     new_stop.to_unix_timestamp());
  try_step_statement(update_entrystaging_stop);
}

void DB_SQLite::edit_entrystaging_location_name
    (const std::string &new_location_name) {
  sqlite3_reset(update_entrystaging_location);
  sqlite3_bind_text(update_entrystaging_location, 1, new_location_name.c_str(),
                    new_location_name.size(), SQLITE_STATIC);
  try_step_statement(update_entrystaging_location);
}

void DB_SQLite::delete_task(Id task_id) {
  sqlite3_reset(remove_task);
  sqlite3_bind_int64(remove_task, 1, task_id);
  try_step_statement(remove_task);
}

void DB_SQLite::delete_project(Id project_id) {
  sqlite3_reset(remove_project);
  sqlite3_bind_int64(remove_project, 1, project_id);
  try_step_statement(remove_project);
}

void DB_SQLite::delete_location(Id location_id) {
  sqlite3_reset(remove_location);
  sqlite3_bind_int64(remove_location, 1, location_id);
  try_step_statement(remove_location);
}

void DB_SQLite::delete_entry(Id entry_id) {
  sqlite3_reset(remove_entry);
  sqlite3_bind_int64(remove_entry, 1, entry_id);
  try_step_statement(remove_entry);
}

void DB_SQLite::commit_entrystaging(){
  sqlite3_reset(insert_entrystaging);
  try_step_statement(insert_entrystaging);
}

std::vector<ProjectTotal>
DB_SQLite::report_project_totals(const DateRange &date_range) {
  uint64_t start_stamp = date_range.start.to_unix_timestamp();
  uint64_t stop_stamp = date_range.stop.to_unix_timestamp();
  sqlite3_reset(sum_duration_per_project);
  sqlite3_bind_int64(sum_duration_per_project, 1, start_stamp);
  sqlite3_bind_int64(sum_duration_per_project, 2, stop_stamp);
  std::vector<ProjectTotal> totals;
  while (sqlite3_step(sum_duration_per_project) == SQLITE_ROW) {
    std::string project_name = reinterpret_cast<const char*>
      (sqlite3_column_text(sum_duration_per_project, 0));
    uint64_t seconds = sqlite3_column_int64(sum_duration_per_project, 1);
    Duration duration(seconds);
    totals.push_back(ProjectTotal{project_name, duration});
  }
  return totals;
}

WeeklyTotals DB_SQLite::report_weekly_totals(const Date &first_day_start) {
  log("report_weekly_totals starting on: " + first_day_start.to_string());
  WeeklyTotals totals;
  auto day_start = first_day_start;
  auto day_stop = day_start;
  day_stop.add_one_day();
  DateRange cur_day (day_start, day_stop);
  for (std::size_t i = 0 ; i < totals.daily_totals.size() ; i++) {
    totals.daily_totals.at(i) = query_entries_duration(cur_day);
    cur_day.add_one_day();
  }
  return totals;
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

void DB_SQLite::try_step_statement(sqlite3_stmt *stmt) {
  try {
    sqlite_db.step_statement(stmt);
  } catch (SQLiteConstraintExcept &e) {
    std::string msg = "DB logic error!\n";
    msg += e.what();
    throw DBLogicExcept(msg.c_str());
  }
}
