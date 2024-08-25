/** @file
 * @brief DB interface implementation using SQLite3. */
#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include "../db_interface.h"
#include "db_sqlite_handle.h"
#include <algorithm>
#include <iterator>

/** @brief DB implementation using SQLite3. */
class DB_SQLite : public DB_Interface {
public:
  /** @brief Open the DB handle. */
  DB_SQLite(const std::filesystem::path &db_file);
  /** @brief Finalize persistent statements etc. */
  ~DB_SQLite();

  std::vector<Project> query_projects() override;
  std::vector<Task> query_tasks(Id project_id) override;
  std::vector<Entry> query_entries(const DateRange &date_range) override;
  Duration query_entries_duration(const DateRange &date_range) override;
  EntryStaging query_entrystaging () override;
  void add_project(std::string project_name) override;
  void add_task(Id project_id, std::string task_name) override;
  void add_entry(Id task_id, const Date &start, const Date &stop) override;
  void edit_project_name(Id project_id, std::string new_project_name) override;
  void edit_task_name(Id task_id, std::string new_task_name) override;
  void edit_entry_project(Id entry_id,
                          const std::string &new_project_name) override;
  void edit_entry_task(Id entry_id, const std::string &new_task_name) override;
  void edit_entry_start(Id entry_id, const Date &new_start_date) override;
  void edit_entry_stop(Id entry_id, const Date &new_stop_date) override;
  void edit_entrystaging_project_name
      (const std::string &new_project_name) override;
  void edit_entrystaging_task_name(const std::string &new_task_name) override;
  void edit_entrystaging_start(const Date &new_start) override;
  void edit_entrystaging_stop(const Date &new_stop) override;
  void delete_task(Id task_id) override;
  void delete_project(Id project_id) override;
  void delete_entry(Id entry_id) override;
  void commit_entrystaging() override;

private:
  /** @brief Low-level handle to the DB. */
  DB_SQLite_Handle sqlite_db;
  /** @brief Statement for querying the projects list. */
  sqlite3_stmt *select_projects;

  /** @brief Create the SQL table for projects. */
  void create_projects_table();
  /** @brief Create the SQL table for tasks. */
  void create_tasks_table();
  /** @brief Create the SQL table for entries. */
  void create_entries_table();
  /** @brief Create an index over the start date of entries. */
  void create_entries_start_index();
  /** @brief Create the SQL table for entrystaging. */
  void create_entrystaging_table();
  /** @brief Execute a SQL statement with exception catching. */
  void try_exec_statement(const std::string &statement);

  /** @brief Convert a NameRows object to a GenericItem. */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of<GenericItem, T>::value>>
  std::vector<T> convert_namerows (const NameRows &rows) {
    std::vector<T> items;
    items.reserve(rows.size());
    std::transform(rows.begin(), rows.end(),
                   std::back_inserter(items),
                   [](const auto &pair) {return T{pair.first, pair.second}; });
    return items;
  }
};

#endif // DB_SQLITE_H
