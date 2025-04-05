#include "db_sqlite.h"
#include "core/project_total.h"
#include "db_lib/db_sqlite_handle.h"
#include <optional>
#include <string>
#include "log_lib/logger.h"
#include "version/version.h"

namespace db {
  DB_SQLite &DB_SQLite::get(const std::filesystem::path &db_filepath) {
    static DB_SQLite instance (db_filepath);
    return instance;
  }

  DB_SQLite::DB_SQLite(const std::filesystem::path &db_file)
    : sqlite_db(std::make_shared<db_lib::DB_SQLite_Handle>(db_file)),
      statements{init_db()} {
    sqlite_db->check_user_version(version::TIMESHEETING_DB_VERSION);
    log_lib::Logger::get().log("Loaded DB v"
                               + std::to_string(sqlite_db->get_user_version()),
                               log_lib::LogLevel::info);
  }

  StatementSet DB_SQLite::init_db() {
    this->create_projects_table();
    this->create_tasks_table();
    this->create_locations_table();
    this->create_entries_table();
    this->create_entries_start_index();
    this->create_entries_update_trigger();
    this->create_entries_insert_trigger();
    this->create_entrystaging_table();
    return StatementSet(sqlite_db);
  }

  std::vector<core::Project> DB_SQLite::query_projects() {
    return query_generic_items<core::Project>(statements.select_projects);
  }

  std::vector<core::Project> DB_SQLite::query_projects_active() {
    return query_generic_items<core::Project>
      (statements.select_projects_active);
  }

  std::vector<core::Task> DB_SQLite::query_tasks(core::Id project_id) {
    statements.select_tasks.bind_all(project_id);
    return query_generic_items<core::Task>(statements.select_tasks);
  }

  std::vector<core::Task> DB_SQLite::query_tasks_active(core::Id project_id) {
    statements.select_tasks_active.bind_all(project_id);
    return query_generic_items<core::Task>(statements.select_tasks_active);
  }

  std::vector<core::Location> DB_SQLite::query_locations() {
    return query_generic_items<core::Location>(statements.select_locations);
  }

  std::vector<core::Location> DB_SQLite::query_locations_active() {
    return query_generic_items<core::Location>
      (statements.select_locations_active);
  }

  std::vector<core::Entry> DB_SQLite::query_entries
  (const time_lib::DateRange &date_range) {
    auto &stmt = statements.select_entries;
    stmt.bind_all(date_range.start.to_unix_timestamp(),
                  date_range.stop.to_unix_timestamp());
    std::vector<core::Entry> vec;
    while (stmt.step()) {
      auto [id, project_name, task_name, start_unix, stop_unix, location_name] =
        stmt.get_all<db_lib::RowId, std::string, std::string, db_lib::DBInt, db_lib::DBInt,
                     std::string>();
      time_lib::Date start_date(start_unix);
      time_lib::Date stop_date(stop_unix);
      core::Entry e{id, project_name, task_name,
                    start_date, stop_date, location_name};
      vec.push_back(e);
    }
    return vec;
  }

  core::Id DB_SQLite::query_entry_project_id(core::Id entry_id) {
    auto &stmt = statements.select_entry_project_id;
    stmt.bind_all(entry_id);
    auto [project_id] = stmt.single_get_all<db_lib::DBInt>();
    return project_id;
  }

  std::generator<core::ExportRow> DB_SQLite::query_export_entries
  (const time_lib::DateRange &date_range) {
    auto &stmt = statements.select_export_entries;
    stmt.bind_all(date_range.start.to_unix_timestamp(),
                  date_range.stop.to_unix_timestamp());
    while (stmt.step()) {
      auto [entry_id, project_id, project_name, task_id, task_name, location_id,
            location_name, start_date_stamp, stop_date_stamp] =
        stmt.get_all<db_lib::RowId, db_lib::RowId, std::string,
                     db_lib::RowId, std::string, db_lib::RowId,
                     std::string, db_lib::DBInt, db_lib::DBInt>();
      core::ExportRow r{entry_id, project_id, project_name, task_id, task_name,
                        location_id, location_name,
                        time_lib::Date(start_date_stamp),
                        time_lib::Date(stop_date_stamp)};
      co_yield r;
    }
  }

  time_lib::Duration DB_SQLite::query_entries_duration
  (const time_lib::DateRange &date_range) {
    auto &stmt = statements.select_duration;
    stmt.bind_all(date_range.start.to_unix_timestamp(),
                  date_range.stop.to_unix_timestamp());
    auto [total_seconds] = stmt.single_get_all<db_lib::DBInt>();
    return time_lib::Duration(total_seconds);
  }

  core::EntryStaging DB_SQLite::query_entrystaging() {
    auto &stmt = statements.select_entrystaging;
    auto [project_name_ret, task_name_ret, start_unix, stop_unix,
          location_name_ret]
      = stmt.single_get_all<std::string, std::string, db_lib::DBInt, db_lib::DBInt,
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
    std::optional<time_lib::Date> start_date;
    if (start_unix == 0)
      start_date = std::nullopt;
    else
      start_date = time_lib::Date(start_unix);
    std::optional<time_lib::Date> stop_date;
    if (stop_unix == 0)
      stop_date = std::nullopt;
    else
      stop_date = time_lib::Date(stop_unix);
    std::optional<std::string> location_name;
    if (location_name_ret.empty())
      location_name = std::nullopt;
    else
      location_name = location_name_ret;
    return core::EntryStaging{project_name, task_name, start_date, stop_date,
                              location_name};
  }

  core::Id DB_SQLite::query_entrystaging_project_id() {
    auto &stmt = statements.select_entrystaging_project_id;
    auto [project_id] = stmt.single_get_all<db_lib::DBInt>();
    return project_id;
  }

  void DB_SQLite::create_projects_table() {
    std::string create_projects_table_st =
      "CREATE TABLE IF NOT EXISTS projects ("
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
      "stop INTEGER NOT NULL UNIQUE, "
      "location_id INTEGER NOT NULL, "
      "FOREIGN KEY (task_id) REFERENCES tasks (id), "
      "FOREIGN KEY (location_id) REFERENCES locations (id), "
      "CHECK (start < stop) "
      ");";
    sqlite_db->exec_statement(create_entries_table_st);
  }

  void DB_SQLite::create_entries_start_index() {
    std::string create_entries_start_index_st =
      "CREATE UNIQUE INDEX IF NOT EXISTS idx_entries_start "
      "ON entries (start);";
    sqlite_db->exec_statement(create_entries_start_index_st);
  }

  void DB_SQLite::create_entries_update_trigger() {
    std::string create_entries_update_trigger_st =
      "CREATE TRIGGER IF NOT EXISTS entries_update_trigger "
      "BEFORE UPDATE "
      "ON entries "
      "BEGIN "
      "SELECT CASE WHEN "
      "EXISTS("
      "SELECT * FROM entries "
      "WHERE NEW.start < entries.stop AND NEW.stop > entries.start "
      "AND entries.start != OLD.start "
      "LIMIT 1"
      ") "
      "THEN "
      "RAISE (ABORT, 'Overlapping entry detected.') "
      "END; " // Case
      "END;";
    sqlite_db->exec_statement(create_entries_update_trigger_st);
  }

  void DB_SQLite::create_entries_insert_trigger() {
    std::string create_entries_insert_trigger_st =
      "CREATE TRIGGER IF NOT EXISTS entries_insert_trigger "
      "BEFORE INSERT "
      "ON entries "
      "BEGIN "
      "SELECT CASE WHEN "
      "EXISTS("
      "SELECT * FROM entries "
      "WHERE NEW.start < entries.stop AND NEW.stop > entries.start "
      "LIMIT 1"
      ") "
      "THEN "
      "RAISE (ABORT, 'Overlapping entry detected.') "
      "END; " // Case
      "END;";
    sqlite_db->exec_statement(create_entries_insert_trigger_st);
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
    sqlite_db->exec_statement(insert_row_st);
  }

  bool DB_SQLite::add_project(std::string project_name) {
    auto &stmt = statements.insert_project;
    stmt.bind_all(project_name);
    return stmt.execute();
  }

  bool DB_SQLite::add_task(core::Id project_id, std::string task_name) {
    auto &stmt = statements.insert_task;
    stmt.bind_all(project_id, task_name);
    return stmt.execute();
  }

  bool DB_SQLite::add_location(const std::string &location_name) {
    auto &stmt = statements.insert_location;
    stmt.bind_all(location_name);
    return stmt.execute();
  }

  bool DB_SQLite::edit_project_name(core::Id project_id,
                                    std::string new_project_name) {
    auto &stmt = statements.update_project_name;
    stmt.bind_all(new_project_name, project_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_task_name(core::Id task_id, std::string new_task_name) {
    auto &stmt = statements.update_task_name;
    stmt.bind_all(new_task_name, task_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_task_project(core::Id task_id,
                                    std::string project_name) {
    auto &stmt = statements.update_task_project;
    stmt.bind_all(project_name, task_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_location_name(core::Id location_id,
                                     const std::string &new_location_name) {
    auto &stmt = statements.update_location_name;
    stmt.bind_all(new_location_name, location_id);
    return stmt.execute();
  }

  bool DB_SQLite::toggle_location_active(core::Id location_id) {
    auto &stmt = statements.toggle_location_flag;
    stmt.bind_all(location_id);
    return stmt.execute();
  }

  bool DB_SQLite::toggle_task_active(core::Id task_id) {
    auto &stmt = statements.toggle_task_flag;
    stmt.bind_all(task_id);
    return stmt.execute();
  }

  bool DB_SQLite::toggle_project_active(core::Id project_id) {
    auto &stmt = statements.toggle_project_flag;
    stmt.bind_all(project_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_entry_project(core::Id entry_id,
                                     const std::string &new_project_name) {
    auto &stmt = statements.update_entry_project;
    stmt.bind_all(new_project_name, entry_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_entry_task(core::Id entry_id,
                                  const std::string &new_task_name) {
    auto &stmt = statements.update_entry_task;
    stmt.bind_all(new_task_name, entry_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_entry_start(core::Id entry_id,
                                   const time_lib::Date &new_start_date) {
    auto &stmt = statements.update_entry_start;
    stmt.bind_all(new_start_date.to_unix_timestamp(), entry_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_entry_stop(core::Id entry_id,
                                  const time_lib::Date &new_stop_date) {
    auto &stmt = statements.update_entry_stop;
    stmt.bind_all(new_stop_date.to_unix_timestamp(), entry_id);
    return stmt.execute();
  }

  bool DB_SQLite::edit_entry_location(core::Id entry_id,
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

  bool DB_SQLite::edit_entrystaging_start(const time_lib::Date &new_start) {
    auto &stmt = statements.update_entrystaging_start;
    stmt.bind_all(new_start.to_unix_timestamp());
    return stmt.execute();
  }

  bool DB_SQLite::edit_entrystaging_stop(const time_lib::Date &new_stop) {
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

  bool DB_SQLite::delete_task(core::Id task_id) {
    auto &stmt = statements.remove_task;
    stmt.bind_all(task_id);
    return stmt.execute();
  }

  bool DB_SQLite::delete_project(core::Id project_id) {
    auto &stmt = statements.remove_project;
    stmt.bind_all(project_id);
    return stmt.execute();
  }

  bool DB_SQLite::delete_location(core::Id location_id) {
    auto &stmt = statements.remove_location;
    stmt.bind_all(location_id);
    return stmt.execute();
  }

  bool DB_SQLite::delete_entry(core::Id entry_id) {
    auto &stmt = statements.remove_entry;
    stmt.bind_all(entry_id);
    return stmt.execute();
  }

  bool DB_SQLite::commit_entrystaging(){
    return statements.insert_entrystaging.execute();
  }

  std::vector<core::ProjectTotal>
  DB_SQLite::report_project_totals(const time_lib::DateRange &date_range) {
    std::vector<core::ProjectTotal> project_totals;
    // Per-project loop
    auto &stmt_per_project = statements.duration_per_worked_project;
    stmt_per_project.bind_all(date_range.start.to_unix_timestamp(),
                              date_range.stop.to_unix_timestamp());
    while(stmt_per_project.step()) {
      core::ProjectTotal project_total;
      auto [project_id, project_name, project_seconds] =
          stmt_per_project.get_all<db_lib::RowId, std::string, db_lib::DBInt>();
      project_total.project_name = project_name;
      project_total.total = time_lib::Duration(project_seconds);
      // Per-task loop
      auto &stmt_per_task = statements.duration_per_worked_task;
      stmt_per_task.bind_all(project_id,
                             date_range.start.to_unix_timestamp(),
                             date_range.stop.to_unix_timestamp());
      while (stmt_per_task.step()) {
        core::TaskTotal task_total;
        auto [task_id, task_name, task_seconds] =
          stmt_per_task.get_all<db_lib::RowId, std::string, db_lib::DBInt>();
        task_total.task_name = task_name;
        task_total.total = time_lib::Duration(task_seconds);
        project_total.task_totals.push_back(task_total);
      }
      project_totals.push_back(project_total);
    }
    return project_totals;
  }

  time_lib::Duration DB_SQLite::report_project_duration
  (core::Id project_id, const time_lib::DateRange &date_range) {
    auto &stmt = statements.project_duration;
    stmt.bind_all(project_id,
                  date_range.start.to_unix_timestamp(),
                  date_range.stop.to_unix_timestamp());
    auto [seconds] = stmt.single_get_all<db_lib::DBInt>();
    return time_lib::Duration(seconds);
  }

  time_lib::Duration DB_SQLite::report_task_duration
  (core::Id task_id, const time_lib::DateRange &date_range) {
    auto &stmt = statements.task_duration;
    stmt.bind_all(task_id,
                  date_range.start.to_unix_timestamp(),
                  date_range.stop.to_unix_timestamp());
    auto [seconds] = stmt.single_get_all<db_lib::DBInt>();
    return time_lib::Duration(seconds);
  }

  core::WeeklyTotals DB_SQLite::report_weekly_totals
  (const time_lib::Week &week) {
    log_lib::logger().log("report_weekly_totals for week over date range: " +
                          week.to_date_range().start.to_string() + " ; " +
                          week.to_date_range().stop.to_string(),
                          log_lib::LogLevel::debug);
    core::WeeklyTotals totals;
    // Overall weekly total
    totals.total = query_entries_duration(week.to_date_range());
    // Daily totals
    // TODO: find a way to avoid the explicit i.
    std::size_t i = 0;
    for (const auto &day : week.days()) {
      totals.daily_totals.at(i) = query_entries_duration(day.to_date_range());
      ++i;
    }
    // Per-project totals
    auto &stmt_per_project = statements.duration_per_worked_project;
    stmt_per_project.bind_all(week.to_date_range().start.to_unix_timestamp(),
                              week.to_date_range().stop.to_unix_timestamp());
    while(stmt_per_project.step()) {
      core::PerProjectTotals per_project_totals;
      auto [project_id, project_name, project_seconds] =
        stmt_per_project.get_all<db_lib::RowId, std::string, db_lib::DBInt>();
      per_project_totals.project_name = project_name;
      per_project_totals.total = time_lib::Duration(project_seconds);
      // Daily totals for the current project (by project_id)
      i = 0;
      for (const auto &day : week.days()) {
        per_project_totals.daily_totals.at(i) =
          report_project_duration(project_id, day.to_date_range());
        ++i;
      }
      auto &stmt_per_task = statements.duration_per_worked_task;
      stmt_per_task.bind_all(project_id,
                             week.to_date_range().start.to_unix_timestamp(),
                             week.to_date_range().stop.to_unix_timestamp());
      while (stmt_per_task.step()) {
        core::PerTaskTotals per_task_totals;
        auto [task_id, task_name, task_seconds] =
          stmt_per_task.get_all<db_lib::RowId, std::string, db_lib::DBInt>();
        per_task_totals.task_name = task_name;
        per_task_totals.total = time_lib::Duration(task_seconds);
        // Daily totals for the current task.
        i = 0;
        for (const auto &day : week.days()) {
          per_task_totals.daily_totals.at(i) =
            report_task_duration(task_id, day.to_date_range());
          ++i;
        }
        per_project_totals.task_totals.push_back(per_task_totals);
      }
      totals.project_totals.push_back(per_project_totals);
    }
    return totals;
  }

  DB_SQLite& db() { return DB_SQLite::get(); }
}
