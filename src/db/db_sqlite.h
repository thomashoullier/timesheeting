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
#include <type_traits>

/** @brief DB implementation using SQLite3. */
class DB_SQLite {
public:
  /** @brief Grab the DB singleton. */
  static DB_SQLite& get();

  std::vector<Project> query_projects();
  std::vector<Project> query_projects_active();
  std::vector<Task> query_tasks(Id project_id);
  std::vector<Task> query_tasks_active(Id project_id);
  std::vector<Location> query_locations();
  std::vector<Location> query_locations_active();
  std::vector<Entry> query_entries(const DateRange &date_range);
  Duration query_entries_duration(const DateRange &date_range);
  EntryStaging query_entrystaging ();
  bool add_project(std::string project_name);
  bool add_task(Id project_id, std::string task_name);
  bool add_location(const std::string &location_name);
  bool edit_project_name(Id project_id, std::string new_project_name);
  bool edit_task_name(Id task_id, std::string new_task_name);
  bool edit_location_name(Id location_id,
                          const std::string &new_location_name);
  bool toggle_location_active(Id location_id);
  bool toggle_task_active(Id task_id);
  bool toggle_project_active(Id project_id);
  bool edit_entry_project(Id entry_id,
                          const std::string &new_project_name);
  bool edit_entry_task(Id entry_id, const std::string &new_task_name);
  bool edit_entry_start(Id entry_id, const Date &new_start_date);
  bool edit_entry_stop(Id entry_id, const Date &new_stop_date);
  bool edit_entry_location(Id entry_id,
                           const std::string &new_location_name);
  bool edit_entrystaging_project_name (const std::string &new_project_name);
  bool edit_entrystaging_task_name(const std::string &new_task_name);
  bool edit_entrystaging_start(const Date &new_start);
  bool edit_entrystaging_stop(const Date &new_stop);
  bool edit_entrystaging_location_name (const std::string &new_location_name);
  bool delete_task(Id task_id);
  bool delete_project(Id project_id);
  bool delete_location (Id location_id);
  bool delete_entry(Id entry_id);
  bool commit_entrystaging();
  std::vector<ProjectTotal> report_project_totals(const DateRange &date_range);
  WeeklyTotals report_weekly_totals (const Date &first_day_start);

private:
  /** @brief Open the DB handle. */
  DB_SQLite(const std::filesystem::path &db_file);
  /** @brief Low-level handle to the DB. */
  std::shared_ptr<DB_SQLite_Handle> sqlite_db;
  /** @brief Set of all used SQLite statements. */
  StatementSet statements;

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
  /** @brief Query template for GenericItems. */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<GenericItem, T>>>
  std::vector<T> query_generic_items(Statement &statement);
};

template <typename T, class>
std::vector<T> DB_SQLite::query_generic_items(Statement &statement) {
  std::vector<T> items;
  while (statement.step()) {
    auto [id, name] = statement.get_all<RowId, std::string>();
    items.push_back(T{id, name});
  }
  return items;
}

/** @brief Grab the DB. */
DB_SQLite &db();

#endif // DB_SQLITE_H
