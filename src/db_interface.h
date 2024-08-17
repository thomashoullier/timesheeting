/** @file
 * @brief DB virtual interface definition. */
#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <exception>
#include <string>
#include <vector>
#include "data_objects.h"

/** @brief Virtual interface class for database. */
class DB_Interface {
public:
  virtual ~DB_Interface() = default;
  /** @brief Return the set of projects stored in the DB. */
  virtual std::vector<Project> query_projects() = 0;
  /** @brief Return the set of tasks for a given project. */
  virtual std::vector<Task> query_tasks (Id project_id) = 0;

  /** @brief Add a project to the DB with the provided name. */
  virtual void add_project(std::string project_name) = 0;
  /** @brief Add a task to a given project with the provided task name. */
  virtual void add_task(Id project_id, std::string task_name) = 0;
  /** @brief Add an entry for a given task with a start and stop dates. */
  virtual void add_entry(Id task_id, const Date &start, const Date &stop) = 0;
  /** @brief Edit a project's name. */
  virtual void edit_project_name(Id project_id,
                                 std::string new_project_name) = 0;
  /** @brief Edit a task's name. */
  virtual void edit_task_name(Id task_id,
                              std::string new_task_name) = 0;
  /** @brief Delete the task with provided Id. */
  virtual void delete_task (Id task_id) = 0;
  /** @brief Delete the project with provided Id and all associated tasks. */
  virtual void delete_project (Id project_id) = 0;
};

/** @brief Exception thrown when the DB encounters a logic error. */
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
