#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <string>
#include <vector>
#include <stdexcept>
#include "sqlite3.h"
#include "data_objects.h"

// TODO: * Add a backend class for generic sql operation wrapping.
//       * Transform this class into the timesheeting-specific operations
//       we need.
class DB_Interface {
public:
  DB_Interface (std::string db_file);
  ~DB_Interface ();

  std::vector<Project> query_projects();
  std::vector<Task> query_tasks (Id project_id);

  void create_projects_table();
  void add_project(std::string project_name);
  void edit_project_name(Id project_id, std::string new_project_name);

private:
  sqlite3 *db;
};

#endif // DB_INTERFACE_H
