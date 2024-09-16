/** @file
 * @brief DB interface implementation using SQLite3. */
#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include <exception>
#include <string>
#include "../data_objects/data_objects.h"
#include "../data_objects/date_range.h"
#include "../data_objects/project_total.h"
#include "../data_objects/weekly_totals.h"
#include "db_sqlite_handle.h"
#include "statement_set.h"
#include <algorithm>
#include <iterator>
#include <type_traits>

/** @brief Exception thrown when the DB encounters a logic error. */
class DBLogicExcept : public std::exception {
private:
  /** @brief Exception message. */
  std::string msg;

public:
  /** @brief Exception constructor. */
  DBLogicExcept (const char* _msg) : msg(_msg) {}
  /** @brief Mandatory std::exception method. */
  const char* what() const throw() {
    return msg.c_str();
  }
};

/** @brief DB implementation using SQLite3. */
class DB_SQLite {
public:
  /** @brief Grab the DB singleton. */
  static DB_SQLite& get();
  /** @brief Finalize persistent statements etc. */
  ~DB_SQLite();

  std::vector<Project> query_projects();
  std::vector<Project> query_projects_active();
  std::vector<Task> query_tasks(Id project_id);
  std::vector<Task> query_tasks_active(Id project_id);
  std::vector<Location> query_locations();
  std::vector<Location> query_locations_active();
  std::vector<Entry> query_entries(const DateRange &date_range);
  Duration query_entries_duration(const DateRange &date_range);
  EntryStaging query_entrystaging ();
  void add_project(std::string project_name);
  void add_task(Id project_id, std::string task_name);
  void add_location(const std::string &location_name);
  void add_entry(Id task_id, const Date &start, const Date &stop,
                 Id location_id);
  void edit_project_name(Id project_id, std::string new_project_name);
  void edit_task_name(Id task_id, std::string new_task_name);
  void edit_location_name(Id location_id,
                          const std::string &new_location_name);
  void toggle_location_active(Id location_id);
  void toggle_task_active(Id task_id);
  void toggle_project_active(Id project_id);
  void edit_entry_project(Id entry_id,
                          const std::string &new_project_name);
  void edit_entry_task(Id entry_id, const std::string &new_task_name);
  void edit_entry_start(Id entry_id, const Date &new_start_date);
  void edit_entry_stop(Id entry_id, const Date &new_stop_date);
  void edit_entry_location(Id entry_id,
                           const std::string &new_location_name);
  void edit_entrystaging_project_name (const std::string &new_project_name);
  void edit_entrystaging_task_name(const std::string &new_task_name);
  void edit_entrystaging_start(const Date &new_start);
  void edit_entrystaging_stop(const Date &new_stop);
  void edit_entrystaging_location_name (const std::string &new_location_name);
  void delete_task(Id task_id);
  void delete_project(Id project_id);
  void delete_location (Id location_id);
  void delete_entry(Id entry_id);
  void commit_entrystaging();
  std::vector<ProjectTotal> report_project_totals(const DateRange &date_range);
  WeeklyTotals report_weekly_totals (const Date &first_day_start);

private:
  /** @brief Open the DB handle. */
  DB_SQLite(const std::filesystem::path &db_file);
  /** @brief Low-level handle to the DB. */
  std::shared_ptr<DB_SQLite_Handle> sqlite_db;
  /** @brief Set of all used SQLite statements. */
  StatementSet statements;
  /** @brief Statement for querying the entry contained in entrystaging. */
  sqlite3_stmt *select_entrystaging;
  /** @brief Statement for adding a project. */
  sqlite3_stmt *insert_project;
  /** @brief Statement for adding a task. */
  sqlite3_stmt *insert_task;
  /** @brief Statement for adding a location. */
  sqlite3_stmt *insert_location;
  /** @brief Statement for adding an entry. */
  sqlite3_stmt *insert_entry;
  /** @brief Statement for editing a project's name. */
  sqlite3_stmt *update_project_name;
  /** @brief Statement for editing a task's name. */
  sqlite3_stmt *update_task_name;
  /** @brief Statement for editing a location's name. */
  sqlite3_stmt *update_location_name;
  /** @brief Statement for toggling the location active flag. */
  sqlite3_stmt *toggle_location_flag;
  /** @brief Statement for toggling the task active flag. */
  sqlite3_stmt *toggle_task_flag;
  /** @brief Statement for toggling the project active flag. */
  sqlite3_stmt *toggle_project_flag;
  /** @brief Statement for editing an entry's project. */
  sqlite3_stmt *update_entry_project;
  /** @brief Statement for editing an entry's task. */
  sqlite3_stmt *update_entry_task;
  /** @brief Statement for editing an entry's start date. */
  sqlite3_stmt *update_entry_start;
  /** @brief Statement for editing an entry's stop date. */
  sqlite3_stmt *update_entry_stop;
  /** @brief Statement for editing an entry's location. */
  sqlite3_stmt *update_entry_location;
  /** @brief Statement for editing the entrystaging project name. */
  sqlite3_stmt *update_entrystaging_project;
  /** @brief Statement for editing the entrystaging task name. */
  sqlite3_stmt *update_entrystaging_task;
  /** @brief Statement for editing the entrystaging start date. */
  sqlite3_stmt *update_entrystaging_start;
  /** @brief Statement for editing the entrystaging stop date. */
  sqlite3_stmt *update_entrystaging_stop;
  /** @brief Statement for editing the entrystaging location. */
  sqlite3_stmt *update_entrystaging_location;
  /** @brief Statement for deleting a task. */
  sqlite3_stmt *remove_task;
  /** @brief Statement for deleting a project. */
  sqlite3_stmt *remove_project;
  /** @brief Statement for deleting a location. */
  sqlite3_stmt *remove_location;
  /** @brief Statement for deleting an entry. */
  sqlite3_stmt *remove_entry;
  /** @brief Statement for committing the entrystaging to entries. */
  sqlite3_stmt *insert_entrystaging;
  /** @brief Sum duration per project for a given date range. */
  sqlite3_stmt *sum_duration_per_project;
  /** @brief Sum duration per project actually worked on
      for a given date range. */
  sqlite3_stmt *duration_per_worked_project;
  /** @brief Sum duration per task actually worked on for a given date range,
      on a given project. */
  sqlite3_stmt *duration_per_worked_task;
  /** @brief Sum duration for a given project over a given date range. */
  sqlite3_stmt *project_duration;
  /** @brief Sum duration for a given task over a given date range. */
  sqlite3_stmt *task_duration;

  /** @brief Create the SQL table for projects. */
  void create_projects_table();
  /** @brief Create the SQL table for tasks. */
  void create_tasks_table();
  /** @brief Create the SQL table for locations. */
  void create_locations_table();
  /** @brief Create the SQL table for entries. */
  void create_entries_table();
  /** @brief Create an index over the start date of entries. */
  void create_entries_start_index();
  /** @brief Create the SQL table for entrystaging. */
  void create_entrystaging_table();
  /** @brief Sum duration for a given project over a given date range. */
  Duration report_project_duration (Id project_id, const DateRange &date_range);
  /** @brief Sum duration for a given task over a given date range. */
  Duration report_task_duration (Id task_id, const DateRange &date_range);
  /** @brief Execute a SQL statement with exception catching. */
  void try_exec_statement(const std::string &statement);
  /** @brief Step a SQL statement with exception catching. */
  void try_step_statement(sqlite3_stmt *stmt);
  /** @brief Query template for GenericItems. */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<GenericItem, T>>>
  std::vector<T> query_generic_items(Statement &statement);
};

/** @brief Grab the DB. */
DB_SQLite &db();

template <typename T, class>
std::vector<T> DB_SQLite::query_generic_items(Statement &statement) {
  std::vector<T> items;
  while(statement.step()) {
    auto [id, name] = statement.get_all<RowId, std::string>();
    items.push_back(T{id, name});
  }
  return items;
}

#endif // DB_SQLITE_H
