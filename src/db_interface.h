#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <exception>
#include <string>
#include <vector>
#include "data_objects.h"

class DB_Interface {
public:
  virtual ~DB_Interface() = default;
  virtual std::vector<Project> query_projects() = 0;
  virtual std::vector<Task> query_tasks (Id project_id) = 0;

  virtual void add_project(std::string project_name) = 0;
  virtual void add_task(Id project_id, std::string task_name) = 0;
  virtual void edit_project_name(Id project_id,
                                 std::string new_project_name) = 0;
  virtual void edit_task_name(Id task_id,
                              std::string new_task_name) = 0;
  virtual void delete_task (Id task_id) = 0;
  /** Delete a project and all associated tasks. */
  virtual void delete_project (Id project_id) = 0;
};

/** Exception thrown when the DB encounters a logic error. */
class DBLogicExcept : public std::exception {
private:
  std::string msg;

public:
  DBLogicExcept (const char* _msg) : msg(_msg) {}
  const char* what() const throw() {
    return msg.c_str();
  }
};

#endif // DB_INTERFACE_H
