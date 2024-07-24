#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <string>
#include <vector>
#include "data_objects.h"

class DB_Interface {
public:
  virtual std::vector<Project> query_projects() = 0;
  virtual std::vector<Task> query_tasks (Id project_id) = 0;

  // TODO: Remove the calls to these methods in our program.
  // void create_projects_table(); // TODO: this is not part of the interface.
  // void create_tasks_table();    // TODO: this is not part of the interface.
  virtual void add_project(std::string project_name) = 0;
  virtual void add_task(Id project_id, std::string task_name) = 0;
  virtual void edit_project_name(Id project_id,
                                 std::string new_project_name) = 0;
};

#endif // DB_INTERFACE_H
