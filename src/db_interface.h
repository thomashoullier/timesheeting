#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <vector>
#include <string>

class DB_Interface {
public:
  std::vector<std::string> query_projects();
  std::vector<std::string> query_tasks (int project_id);
};

#endif // DB_INTERFACE_H
