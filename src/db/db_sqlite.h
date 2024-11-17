/** @file
 * @brief DB interface implementation using SQLite3. */
#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include "time_lib/date_range.h"
#include "time_lib/duration.h"
#include "core/entry.h"
#include "core/entry_staging.h"
#include "core/generic_item.h"
#include "core/project_total.h"
#include "time_lib/week.h"
#include "core/weekly_totals.h"
#include "core/export_row.h"
#include "db_lib/db_sqlite_handle.h"
#include "statement_set.h"
#include <filesystem>
#include <string>
#include <type_traits>

/** @brief DB implementation using SQLite3. */
class DB_SQLite {
public:
  /** @brief Grab the DB singleton. */
  static DB_SQLite &get(const std::filesystem::path &db_filepath = "");

  std::vector<core::Project> query_projects();
  std::vector<core::Project> query_projects_active();
  std::vector<core::Task> query_tasks(core::Id project_id);
  std::vector<core::Task> query_tasks_active(core::Id project_id);
  std::vector<core::Location> query_locations();
  std::vector<core::Location> query_locations_active();
  std::vector<core::Entry> query_entries(const time_lib::DateRange &date_range);
  core::Id query_entry_project_id(core::Id entry_id);
  std::vector<core::ExportRow> query_export_entries
    (const time_lib::DateRange &date_range);
  time_lib::Duration query_entries_duration
    (const time_lib::DateRange &date_range);
  core::EntryStaging query_entrystaging ();
  core::Id query_entrystaging_project_id();
  bool add_project(std::string project_name);
  bool add_task(core::Id project_id, std::string task_name);
  bool add_location(const std::string &location_name);
  bool edit_project_name(core::Id project_id, std::string new_project_name);
  bool edit_task_name(core::Id task_id, std::string new_task_name);
  bool edit_task_project(core::Id task_id, std::string project_name);
  bool edit_location_name(core::Id location_id,
                          const std::string &new_location_name);
  bool toggle_location_active(core::Id location_id);
  bool toggle_task_active(core::Id task_id);
  bool toggle_project_active(core::Id project_id);
  bool edit_entry_project(core::Id entry_id,
                          const std::string &new_project_name);
  bool edit_entry_task(core::Id entry_id, const std::string &new_task_name);
  bool edit_entry_start(core::Id entry_id, const time_lib::Date &new_start_date);
  bool edit_entry_stop(core::Id entry_id, const time_lib::Date &new_stop_date);
  bool edit_entry_location(core::Id entry_id,
                           const std::string &new_location_name);
  bool edit_entrystaging_project_name (const std::string &new_project_name);
  bool edit_entrystaging_task_name(const std::string &new_task_name);
  bool edit_entrystaging_start(const time_lib::Date &new_start);
  bool edit_entrystaging_stop(const time_lib::Date &new_stop);
  bool edit_entrystaging_location_name (const std::string &new_location_name);
  bool delete_task(core::Id task_id);
  bool delete_project(core::Id project_id);
  bool delete_location (core::Id location_id);
  bool delete_entry(core::Id entry_id);
  bool commit_entrystaging();
  std::vector<core::ProjectTotal> report_project_totals
    (const time_lib::DateRange &date_range);
  core::WeeklyTotals report_weekly_totals (const time_lib::Week &week);

private:
  /** @brief Open the DB handle. */
  DB_SQLite(const std::filesystem::path &db_file);
  /** @brief Initialize the DB and provide the ready statement set. */
  StatementSet init_db();
  /** @brief Low-level handle to the DB. */
  std::shared_ptr<db_lib::DB_SQLite_Handle> sqlite_db;
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
  time_lib::Duration report_project_duration
    (core::Id project_id, const time_lib::DateRange &date_range);
  /** @brief Sum duration for a given task over a given date range. */
  time_lib::Duration report_task_duration
    (core::Id task_id, const time_lib::DateRange &date_range);
  /** @brief Query template for GenericItems. */
  template <typename T,
            typename = std::enable_if_t<
              std::is_base_of_v<core::GenericItem, T>>>
  std::vector<T> query_generic_items(db_lib::Statement &statement);
};

template <typename T, class>
std::vector<T> DB_SQLite::query_generic_items(db_lib::Statement &statement) {
  std::vector<T> items;
  while (statement.step()) {
    auto [id, name] = statement.get_all<db_lib::RowId, std::string>();
    items.push_back(T{id, name});
  }
  return items;
}

/** @brief Grab the DB. */
DB_SQLite &db();

#endif // DB_SQLITE_H
