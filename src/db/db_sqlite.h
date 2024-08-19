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
  DB_SQLite (const std::filesystem::path &db_file);

  std::vector<Project> query_projects() override;
  std::vector<Task> query_tasks (Id project_id) override;
  std::vector<Entry> query_entries() override;
  void add_project(std::string project_name) override;
  void add_task(Id project_id, std::string task_name) override;
  void add_entry(Id task_id, const Date &start, const Date &stop) override;
  void edit_project_name(Id project_id, std::string new_project_name) override;
  void edit_task_name(Id task_id, std::string new_task_name) override;
  void edit_entry_start(Id entry_id, const Date &new_start_date) override;
  void edit_entry_stop(Id entry_id, const Date &new_stop_date) override;
  void delete_task(Id task_id) override;
  void delete_project(Id project_id) override;

private:
  /** @brief Handle to the DB. */
  DB_SQLite_Handle sqlite_db;

  /** @brief Create the SQL table for projects. */
  void create_projects_table();
  /** @brief Create the SQL table for tasks. */
  void create_tasks_table();
  /** @brief Create the SQL table for entries. */
  void create_entries_table();
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
