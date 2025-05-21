/** @file
 * @brief DB interface implementation using SQLite3. */
#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include "core/entry.h"
#include "core/entry_staging.h"
#include "core/export_row.h"
#include "core/generic_item.h"
#include "core/id.h"
#include "core/project_total.h"
#include "core/weekly_totals.h"
#include "db_lib/db_types.h"
#include "db_lib/db_sqlite_handle.h"
#include "db_lib/statement.h"
#include "statement_set.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "time_lib/duration.h"
#include "time_lib/week.h"
#include <filesystem>
#include <generator>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include <optional>

namespace db {
  /** @brief DB implementation using SQLite3.

   Singleton pattern. */
  class DB_SQLite {
  public:
    /** @brief Grab the DB singleton. */
    static DB_SQLite &get(const std::filesystem::path &db_filepath = "");

    /** @brief Get the user version of the loaded DB.

     For logging purposes mainly. */
    int get_user_version() const;

    /** @brief Get the list of projects. */
    std::vector<core::Project> query_projects();
    /** @brief Get the list of active projects. */
    std::vector<core::Project> query_projects_active();
    /** @brief Get the list of tasks for a given project. */
    std::vector<core::Task> query_tasks(core::Id project_id);
    /** @brief Get the list of active tasks for a given project. */
    std::vector<core::Task> query_tasks_active(core::Id project_id);
    /** @brief Get the list of locations. */
    std::vector<core::Location> query_locations();
    /** @brief Get the list of active locations. */
    std::vector<core::Location> query_locations_active();
    /** @brief Get the list of entries over a given date range. */
    std::vector<core::Entry> query_entries
    (const time_lib::DateRange &date_range);
    /** @brief Get the project id of a given entry. */
    core::Id query_entry_project_id(core::Id entry_id);
    /** @brief Get the DB export registry of entries over a given date range. */
    std::generator<core::ExportRow> query_export_entries
    (const time_lib::DateRange &date_range);
    /** @brief Get the total duration worked on entries in a given date range. */
    time_lib::Duration query_entries_duration
    (const time_lib::DateRange &date_range);
    /** @brief Get the current entry staging state. */
    core::EntryStaging query_entrystaging ();
    /** @brief Get the project id of the project currently in entry staging. */
    std::optional<core::Id> query_entrystaging_project_id();
    /** @brief Add a project to the DB. */
    bool add_project(std::string project_name);
    /** @brief Add a task to the DB. */
    bool add_task(core::Id project_id, std::string task_name);
    /** @brief Add a location to the DB. */
    bool add_location(const std::string &location_name);
    /** @brief Edit the name of a project. */
    bool edit_project_name(core::Id project_id, std::string new_project_name);
    /** @brief Edit the name of a task. */
    bool edit_task_name(core::Id task_id, std::string new_task_name);
    /** @brief Change the project associated to a given task. */
    bool edit_task_project(core::Id task_id, std::string project_name);
    /** @brief Edit the name of a location. */
    bool edit_location_name(core::Id location_id,
                            const std::string &new_location_name);
    /** @brief Flip the activity flag for a given location. */
    bool toggle_location_active(core::Id location_id);
    /** @brief Flip the activity flag for a given task. */
    bool toggle_task_active(core::Id task_id);
    /** @brief Flip the activity flag for a given project. */
    bool toggle_project_active(core::Id project_id);
    /** @brief Change the project associated to a given entry. */
    bool edit_entry_project(core::Id entry_id,
                            const std::string &new_project_name);
    /** @brief Change the task associated to a given entry. */
    bool edit_entry_task(core::Id entry_id, const std::string &new_task_name);
    /** @brief Change the start date of a given entry. */
    bool edit_entry_start(core::Id entry_id,
                          const time_lib::Date &new_start_date);
    /** @brief Change the stop date of a given entry. */
    bool edit_entry_stop(core::Id entry_id,
                         const time_lib::Date &new_stop_date);
    /** @brief Change the location of a given entry. */
    bool edit_entry_location(core::Id entry_id,
                             const std::string &new_location_name);
    /** @brief Change the project currently in entry staging. */
    bool edit_entrystaging_project_name (const std::string &new_project_name);
    /** @brief Change the task currently in entry staging. */
    bool edit_entrystaging_task_name(const std::string &new_task_name);
    /** @brief Change the start date in entry staging. */
    bool edit_entrystaging_start(const time_lib::Date &new_start);
    /** @brief Change the stop date in entry staging. */
    bool edit_entrystaging_stop(const time_lib::Date &new_stop);
    /** @brief Change the location in entry staging. */
    bool edit_entrystaging_location_name (const std::string &new_location_name);
    /** @brief Delete a given task. */
    bool delete_task(core::Id task_id);
    /** @brief Delete a given project. */
    bool delete_project(core::Id project_id);
    /** @brief Delete a location. */
    bool delete_location (core::Id location_id);
    /** @brief Delete a given entry. */
    bool delete_entry(core::Id entry_id);
    /** @brief Commit the entry staging to the DB. */
    bool commit_entrystaging();
    /** @brief Get a registry of project duration totals
               over a given date range. */
    std::vector<core::ProjectTotal> report_project_totals
    (const time_lib::DateRange &date_range);
    /** @brief Get the weekly report totals. */
    core::WeeklyTotals report_weekly_totals (const time_lib::Week &week);

    DB_SQLite(DB_SQLite const&) = delete;
    void operator=(DB_SQLite const&) = delete;

  private:
    /** @brief Open the DB handle. */
    explicit DB_SQLite(const std::filesystem::path &db_file);
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
    /** @brief Create the update trigger on entries. */
    void create_entries_update_trigger();
    /** @brief Create the insert trigger on entries. */
    void create_entries_insert_trigger();
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
      auto [id, name, active] = statement.get_all<db_lib::RowId, std::string,
                                                  bool>();
      items.push_back(T{id, name, active});
    }
    return items;
  }

  /** @brief Grab the DB. */
  DB_SQLite &db();
} // namespace db

#endif // DB_SQLITE_H
