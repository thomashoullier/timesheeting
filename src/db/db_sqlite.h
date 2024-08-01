#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include "../db_interface.h"
#include "db_sqlite_handle.h"
#include <algorithm>
#include <iterator>

class DB_SQLite : public DB_Interface {
public:
  DB_SQLite (const std::filesystem::path &db_file);

  std::vector<Project> query_projects() override;
  std::vector<Task> query_tasks (Id project_id) override;

  void add_project(std::string project_name) override;
  void add_task(Id project_id, std::string task_name) override;
  void edit_project_name(Id project_id, std::string new_project_name) override;
  void edit_task_name(Id task_id, std::string new_task_name) override;
  void delete_task(Id task_id) override;
  void delete_project(Id project_id) override;

private:
  DB_SQLite_Handle sqlite_db;

  void create_projects_table();
  void create_tasks_table();
  void try_exec_statement(const std::string &statement);

  /** Convert a NameRows object to a GenericItem. */
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
