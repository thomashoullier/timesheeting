#include "db_sqlite.h"
#include "db_sqlite_handle.h"
#include <cstdint>
#include <optional>
#include <string>
#include "../logger/logger.h"

DB_SQLite& DB_SQLite::get() {
  static DB_SQLite instance ("timesheeting.db");
  return instance;
}

DB_SQLite::DB_SQLite(const std::filesystem::path &db_file)
    : sqlite_db(std::make_shared<DB_SQLite_Handle>(db_file)),
      statements(sqlite_db) {
  this->create_projects_table();
  this->create_tasks_table();
  this->create_locations_table();
  this->create_entries_table();
  this->create_entries_start_index();
  this->create_entrystaging_table();

  // Prepare persistent statements
  std::string sum_duration_per_project_st =
    "SELECT projects.name, SUM(entries.stop - entries.start) "
    "FROM projects "
    "INNER JOIN tasks ON projects.id = tasks.project_id "
    "INNER JOIN entries ON tasks.id = entries.task_id "
    "WHERE entries.start >= ? "
    "AND entries.start < ? "
    "GROUP BY projects.name;";
  sum_duration_per_project =
    sqlite_db->prepare_statement(sum_duration_per_project_st);

  std::string duration_per_worked_project_st =
    "SELECT projects.id, projects.name, SUM(entries.stop - entries.start) "
    "FROM projects "
    "INNER JOIN tasks ON projects.id = tasks.project_id "
    "INNER JOIN entries ON tasks.id = entries.task_id "
    "WHERE entries.start >= ? "
    "AND entries.start < ? "
    "GROUP BY projects.name " // TODO: group by id rather no?
    "HAVING SUM(entries.stop - entries.start) > 0;";
  duration_per_worked_project =
      sqlite_db->prepare_statement(duration_per_worked_project_st);

  std::string duration_per_worked_task_st =
    "SELECT tasks.id, tasks.name, SUM(entries.stop - entries.start) "
    "FROM tasks "
    "INNER JOIN entries ON tasks.id = entries.task_id "
    "WHERE tasks.project_id = ? "
    "AND entries.start >= ? "
    "AND entries.start < ? "
    "GROUP BY tasks.name " // TODO: group by id?
    "HAVING SUM(entries.stop - entries.start) > 0;";
  duration_per_worked_task =
    sqlite_db->prepare_statement(duration_per_worked_task_st);

  std::string project_duration_st =
    "SELECT SUM(entries.stop - entries.start) "
    "FROM projects "
    "INNER JOIN tasks ON projects.id = tasks.project_id "
    "INNER JOIN entries ON tasks.id = entries.task_id "
    "WHERE projects.id = ? "
    "AND entries.start >= ? "
    "AND entries.start < ?;";
  project_duration = sqlite_db->prepare_statement(project_duration_st);

  std::string task_duration_st =
    "SELECT SUM(entries.stop - entries.start) "
    "FROM tasks "
    "INNER JOIN entries ON tasks.id = entries.task_id "
    "WHERE tasks.id = ? "
    "AND entries.start >= ? "
    "AND entries.start < ?;";
  task_duration = sqlite_db->prepare_statement(task_duration_st);
}

DB_SQLite::~DB_SQLite() {
  sqlite3_finalize(sum_duration_per_project);
  sqlite3_finalize(duration_per_worked_project);
  sqlite3_finalize(duration_per_worked_task);
  sqlite3_finalize(project_duration);
  sqlite3_finalize(task_duration);
}

std::vector<Project> DB_SQLite::query_projects() {
  return query_generic_items<Project>(statements.select_projects);
}

std::vector<Project> DB_SQLite::query_projects_active() {
  return query_generic_items<Project>(statements.select_projects_active);
}

std::vector<Task> DB_SQLite::query_tasks(Id project_id) {
  statements.select_tasks.bind_all(project_id);
  return query_generic_items<Task>(statements.select_tasks);
}

std::vector<Task> DB_SQLite::query_tasks_active(Id project_id) {
  statements.select_tasks_active.bind_all(project_id);
  return query_generic_items<Task>(statements.select_tasks_active);
}

std::vector<Location> DB_SQLite::query_locations() {
  return query_generic_items<Location>(statements.select_locations);
}

std::vector<Location> DB_SQLite::query_locations_active() {
  return query_generic_items<Location>(statements.select_locations_active);
}

std::vector<Entry> DB_SQLite::query_entries(const DateRange &date_range) {
  auto &stmt = statements.select_entries;
  stmt.bind_all(date_range.start.to_unix_timestamp(),
                date_range.stop.to_unix_timestamp());
  std::vector<Entry> vec;
  while (stmt.step()) {
    auto [id, project_name, task_name, start_unix, stop_unix, location_name] =
      stmt.get_all<RowId, std::string, std::string, uint64_t, uint64_t,
                   std::string>();
    Date start_date(start_unix);
    Date stop_date(stop_unix);
    Entry e{id, project_name, task_name, start_date, stop_date, location_name};
    vec.push_back(e);
  }
  return vec;
}

Duration DB_SQLite::query_entries_duration(const DateRange &date_range) {
  auto &stmt = statements.select_duration;
  stmt.bind_all(date_range.start.to_unix_timestamp(),
                date_range.stop.to_unix_timestamp());
  auto [total_seconds] = stmt.single_get_all<uint64_t>();
  return Duration(total_seconds);
}

EntryStaging DB_SQLite::query_entrystaging() {
  auto &stmt = statements.select_entrystaging;
  auto [project_name_ret, task_name_ret, start_unix, stop_unix,
        location_name_ret]
    = stmt.single_get_all<std::string, std::string, uint64_t, uint64_t,
                          std::string>();
  // Replace NULL values with explicit nullopt.
  std::optional<std::string> project_name;
  if (project_name_ret.empty())
    project_name = std::nullopt;
  else
    project_name = project_name_ret;
  std::optional<std::string> task_name;
  if (task_name_ret.empty())
    task_name = std::nullopt;
  else
    task_name = task_name_ret;
  std::optional<Date> start_date;
  if (start_unix == 0)
    start_date = std::nullopt;
  else
    start_date = Date(start_unix);
  std::optional<Date> stop_date;
  if (stop_unix == 0)
    stop_date = std::nullopt;
  else
    stop_date = Date(stop_unix);
  std::optional<std::string> location_name;
  if (location_name_ret.empty())
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
  sqlite_db->exec_statement(create_projects_table_st);
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
  sqlite_db->exec_statement(create_tasks_table_st);
}

void DB_SQLite::create_locations_table() {
  std::string create_locations_table_st =
    "CREATE TABLE IF NOT EXISTS locations ("
    "id INTEGER PRIMARY KEY, "
    "name TEXT NOT NULL UNIQUE, "
    "active BOOLEAN NOT NULL CHECK (active IN (0,1))"
    ");";
  sqlite_db->exec_statement(create_locations_table_st);
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
  sqlite_db->exec_statement(create_entries_table_st);
}

void DB_SQLite::create_entries_start_index() {
  std::string create_entries_start_index_st =
    "CREATE UNIQUE INDEX IF NOT EXISTS idx_entries_start "
    "ON entries (start);";
  sqlite_db->exec_statement(create_entries_start_index_st);
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
  sqlite_db->exec_statement(create_entrystaging_table_st);

  // Insert a first row if it does not already exist.
  std::string insert_row_st =
    "INSERT INTO entrystaging (task_id, start, stop, location_id) "
    "SELECT NULL, NULL, NULL, NULL "
    "WHERE NOT EXISTS (SELECT * FROM entrystaging)"
    ";";
  try_exec_statement(insert_row_st);
}

bool DB_SQLite::add_project(std::string project_name) {
  auto &stmt = statements.insert_project;
  stmt.bind_all(project_name);
  return stmt.execute();
}

bool DB_SQLite::add_task(Id project_id, std::string task_name) {
  auto &stmt = statements.insert_task;
  stmt.bind_all(project_id, task_name);
  return stmt.execute();
}

bool DB_SQLite::add_location(const std::string &location_name) {
  auto &stmt = statements.insert_location;
  stmt.bind_all(location_name);
  return stmt.execute();
}

bool DB_SQLite::edit_project_name(Id project_id, std::string new_project_name) {
  auto &stmt = statements.update_project_name;
  stmt.bind_all(new_project_name, project_id);
  return stmt.execute();
}

bool DB_SQLite::edit_task_name(Id task_id, std::string new_task_name) {
  auto &stmt = statements.update_task_name;
  stmt.bind_all(new_task_name, task_id);
  return stmt.execute();
}

bool DB_SQLite::edit_location_name(Id location_id,
                                   const std::string &new_location_name) {
  auto &stmt = statements.update_location_name;
  stmt.bind_all(new_location_name, location_id);
  return stmt.execute();
}

bool DB_SQLite::toggle_location_active(Id location_id) {
  auto &stmt = statements.toggle_location_flag;
  stmt.bind_all(location_id);
  return stmt.execute();
}

bool DB_SQLite::toggle_task_active(Id task_id) {
  auto &stmt = statements.toggle_task_flag;
  stmt.bind_all(task_id);
  return stmt.execute();
}

bool DB_SQLite::toggle_project_active(Id project_id) {
  auto &stmt = statements.toggle_project_flag;
  stmt.bind_all(project_id);
  return stmt.execute();
}

bool DB_SQLite::edit_entry_project(Id entry_id,
                                   const std::string &new_project_name) {
  auto &stmt = statements.update_entry_project;
  stmt.bind_all(new_project_name, entry_id);
  return stmt.execute();
}

bool DB_SQLite::edit_entry_task(Id entry_id, const std::string &new_task_name) {
  auto &stmt = statements.update_entry_task;
  stmt.bind_all(new_task_name, entry_id);
  return stmt.execute();
}

bool DB_SQLite::edit_entry_start(Id entry_id, const Date &new_start_date) {
  auto &stmt = statements.update_entry_start;
  stmt.bind_all(new_start_date.to_unix_timestamp(), entry_id);
  return stmt.execute();
}

bool DB_SQLite::edit_entry_stop(Id entry_id, const Date &new_stop_date) {
  auto &stmt = statements.update_entry_stop;
  stmt.bind_all(new_stop_date.to_unix_timestamp(), entry_id);
  return stmt.execute();
}

bool DB_SQLite::edit_entry_location(Id entry_id,
                                    const std::string &new_location_name) {
  auto &stmt = statements.update_entry_location;
  stmt.bind_all(new_location_name, entry_id);
  return stmt.execute();
}

bool DB_SQLite::edit_entrystaging_project_name
    (const std::string &new_project_name) {
  auto &stmt = statements.update_entrystaging_project;
  stmt.bind_all(new_project_name);
  return stmt.execute();
}

bool DB_SQLite::edit_entrystaging_task_name(const std::string &new_task_name) {
  auto &stmt = statements.update_entrystaging_task;
  stmt.bind_all(new_task_name);
  return stmt.execute();
}

bool DB_SQLite::edit_entrystaging_start(const Date &new_start) {
  auto &stmt = statements.update_entrystaging_start;
  stmt.bind_all(new_start.to_unix_timestamp());
  return stmt.execute();
}

bool DB_SQLite::edit_entrystaging_stop(const Date &new_stop) {
  auto &stmt = statements.update_entrystaging_stop;
  stmt.bind_all(new_stop.to_unix_timestamp());
  return stmt.execute();
}

bool DB_SQLite::edit_entrystaging_location_name
    (const std::string &new_location_name) {
  auto &stmt = statements.update_entrystaging_location;
  stmt.bind_all(new_location_name);
  return stmt.execute();
}

bool DB_SQLite::delete_task(Id task_id) {
  auto &stmt = statements.remove_task;
  stmt.bind_all(task_id);
  return stmt.execute();
}

bool DB_SQLite::delete_project(Id project_id) {
  auto &stmt = statements.remove_project;
  stmt.bind_all(project_id);
  return stmt.execute();
}

bool DB_SQLite::delete_location(Id location_id) {
  auto &stmt = statements.remove_location;
  stmt.bind_all(location_id);
  return stmt.execute();
}

bool DB_SQLite::delete_entry(Id entry_id) {
  auto &stmt = statements.remove_entry;
  stmt.bind_all(entry_id);
  return stmt.execute();
}

bool DB_SQLite::commit_entrystaging(){
  return statements.insert_entrystaging.execute();
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
    std::string project_name = reinterpret_cast<const char *>(
        sqlite3_column_text(sum_duration_per_project, 0));
    uint64_t seconds = sqlite3_column_int64(sum_duration_per_project, 1);
    Duration duration(seconds);
    totals.push_back(ProjectTotal{project_name, duration});
  }
  return totals;
}

Duration DB_SQLite::report_project_duration(Id project_id,
                                            const DateRange &date_range) {
  uint64_t start_stamp = date_range.start.to_unix_timestamp();
  uint64_t stop_stamp = date_range.stop.to_unix_timestamp();
  sqlite3_reset(project_duration);
  sqlite3_bind_int64(project_duration, 1, project_id);
  sqlite3_bind_int64(project_duration, 2, start_stamp);
  sqlite3_bind_int64(project_duration, 3, stop_stamp);
  if (sqlite3_step(project_duration) == SQLITE_ROW) {
    uint64_t seconds = sqlite3_column_int64(project_duration, 0);
    Duration duration(seconds);
    return duration;
  } else {
    throw DBLogicExcept("report_project_duration: Cannot query duration!");
  }
}

Duration DB_SQLite::report_task_duration(Id task_id,
                                         const DateRange &date_range) {
  uint64_t start_stamp = date_range.start.to_unix_timestamp();
  uint64_t stop_stamp = date_range.stop.to_unix_timestamp();
  sqlite3_reset(task_duration);
  sqlite3_bind_int64(task_duration, 1, task_id);
  sqlite3_bind_int64(task_duration, 2, start_stamp);
  sqlite3_bind_int64(task_duration, 3, stop_stamp);
  if (sqlite3_step(task_duration) == SQLITE_ROW) {
    uint64_t seconds = sqlite3_column_int64(task_duration, 0);
    Duration duration(seconds);
    return duration;
  } else {
    throw DBLogicExcept("report_task_duration: Cannot query duration!");
  }
}

WeeklyTotals DB_SQLite::report_weekly_totals(const Date &first_day_start) {
  logger().log("report_weekly_totals starting on: " +
             first_day_start.to_string());
  WeeklyTotals totals;
  auto week_start = first_day_start;
  auto week_stop = week_start;
  week_stop.add_one_week();
  // Overall weekly total
  DateRange whole_week(week_start, week_stop);
  totals.total = query_entries_duration(whole_week);
  // Daily totals
  auto day_start = first_day_start;
  auto day_stop = day_start;
  day_stop.add_one_day();
  DateRange cur_day(day_start, day_stop);
  for (std::size_t i = 0; i < totals.daily_totals.size(); i++) {
    totals.daily_totals.at(i) = query_entries_duration(cur_day);
    cur_day.add_one_day();
  }
  // Per-project totals
  sqlite3_reset(duration_per_worked_project);
  DateRange cur_week(week_start, week_stop);
  sqlite3_bind_int64(duration_per_worked_project, 1,
                     cur_week.start.to_unix_timestamp());
  sqlite3_bind_int64(duration_per_worked_project, 2,
                     cur_week.stop.to_unix_timestamp());
  while(sqlite3_step(duration_per_worked_project) == SQLITE_ROW) {
    PerProjectTotals per_project_totals;
    RowId project_id = sqlite3_column_int64(duration_per_worked_project, 0);
    std::string project_name = reinterpret_cast<const char*>
      (sqlite3_column_text(duration_per_worked_project, 1));
    uint64_t seconds = sqlite3_column_int64(duration_per_worked_project, 2);
    Duration duration(seconds);
    per_project_totals.project_name = project_name;
    per_project_totals.total = duration;
    // Daily totals for the current project (by project_id)
    day_start = first_day_start;
    day_stop = day_start;
    day_stop.add_one_day();
    cur_day.start = day_start;
    cur_day.stop = day_stop;
    for (std::size_t i = 0; i < per_project_totals.daily_totals.size(); ++i) {
      duration = report_project_duration(project_id, cur_day);
      per_project_totals.daily_totals.at(i) = duration;
      cur_day.add_one_day();
    }
    sqlite3_reset(duration_per_worked_task);
    sqlite3_bind_int64(duration_per_worked_task, 1, project_id);
    sqlite3_bind_int64(duration_per_worked_task, 2,
                       cur_week.start.to_unix_timestamp());
    sqlite3_bind_int64(duration_per_worked_task, 3,
                       cur_week.stop.to_unix_timestamp());
    while (sqlite3_step(duration_per_worked_task) == SQLITE_ROW) {
      PerTaskTotals per_task_totals;
      RowId task_id = sqlite3_column_int64(duration_per_worked_task, 0);
      std::string task_name = reinterpret_cast<const char *>(
          sqlite3_column_text(duration_per_worked_task, 1));
      uint64_t seconds = sqlite3_column_int64(duration_per_worked_task, 2);
      Duration duration(seconds);
      per_task_totals.task_name = task_name;
      per_task_totals.total = duration;
      // Daily totals for the current task.
      day_start = first_day_start;
      day_stop = day_start;
      day_stop.add_one_day();
      cur_day.start = day_start;
      cur_day.stop = day_stop;
      for (std::size_t i = 0; i < per_task_totals.daily_totals.size(); ++i) {
        duration = report_task_duration(task_id, cur_day);
        per_task_totals.daily_totals.at(i) = duration;
        cur_day.add_one_day();
      }
      per_project_totals.task_totals.push_back(per_task_totals);
    }
    totals.project_totals.push_back(per_project_totals);
  }
  return totals;
}

void DB_SQLite::try_exec_statement(const std::string &statement) {
  try {
    sqlite_db->exec_statement(statement);
  } catch (SQLiteConstraintExcept &e) {
    std::string msg = "DB logic error!\n";
    msg += e.what();
    throw DBLogicExcept(msg.c_str());
  }
}

void DB_SQLite::try_step_statement(sqlite3_stmt *stmt) {
  try {
    sqlite_db->step_statement(stmt);
  } catch (SQLiteConstraintExcept &e) {
    std::string msg = "DB logic error!\n";
    msg += e.what();
    throw DBLogicExcept(msg.c_str());
  }
}

DB_SQLite& db() { return DB_SQLite::get(); }
