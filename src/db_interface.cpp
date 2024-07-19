#include "db_interface.h"

std::vector<Project> DB_Interface::query_projects() {
  Project p1 = {1, "JWST", "X"};
  Project p2 = {2, "Ariane 6", "X"};
  Project p3 = {3, "Tondeuse", "X"};
  Project p4 = {4, "SPICA", "X"};
  Project p5 = {5, "Roman", "X"};
  return {p1, p2, p3, p4, p5};
}

std::vector<Task> DB_Interface::query_tasks(Id project_id) {
  Task t1 = {1, "Kick-off meeting"};
  Task t2 = {2, "Writing specification"};
  Task t3 = {3, "Progress meeting"};
  return {t1, t2, t3};
}
