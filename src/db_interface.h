#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include "data_objects.h"
#include <vector>

class DB_Interface {
public:
  std::vector<Project> query_projects();
  std::vector<Task> query_tasks (Id project_id);
};

#endif // DB_INTERFACE_H
