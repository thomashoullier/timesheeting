#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include "db_interface.h"
#include "sqlite3.h"
#include <stdexcept>

class DB_SQLite : public DB_Interface {
public:
  DB_SQLite (std::string db_file);
  ~DB_SQLite ();

  std::vector<Project> query_projects() override;
  std::vector<Task> query_tasks (Id project_id) override;

  void add_project(std::string project_name) override;
  void add_task(Id project_id, std::string task_name) override;
  void edit_project_name(Id project_id, std::string new_project_name) override;

private:
  sqlite3 *db;

  void create_projects_table();
  void create_tasks_table();
};

#endif // DB_SQLITE_H
