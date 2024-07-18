#include "db_interface.h"

std::vector<std::string> DB_Interface::query_projects() {
  return {"p0", "p1", "p2", "p3", "p4"};
}

std::vector<std::string> DB_Interface::query_tasks(int project_id) {
  return {"z0", "z1", "z2"};
}
