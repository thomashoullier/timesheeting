/** @file
 * @brief DB virtual interface definition. */
#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <exception>
#include <string>
#include <vector>
#include "data_objects.h"
#include "date_range.h"

/** @brief Virtual interface class for database. */
class DB_Interface {
public:
  virtual ~DB_Interface() = default;
  /** @brief Return the set of projects stored in the DB. */
  virtual std::vector<Project> query_projects() = 0;
  /** @brief Return the set of tasks for a given project. */
  virtual std::vector<Task> query_tasks (Id project_id) = 0;
  /** @brief Return the set of entries ordered by ascending start date
      and included in the provided DateRange. */
  virtual std::vector<Entry> query_entries (const DateRange &date_range) = 0;
  /** @brief Get the displayable state of the Entry in staging. */
  virtual EntryStaging query_entrystaging () = 0;

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
  /** @brief Edit an entry's project. */
  virtual void edit_entry_project(Id entry_id,
                                  const std::string &new_project_name) = 0;
  /** @brief Edit an entry's task. */
  virtual void edit_entry_task(Id entry_id,
                               const std::string &new_task_name) = 0;
  /** @brief Edit an entry's start date. */
  virtual void edit_entry_start(Id entry_id,
                                const Date &new_start_date) = 0;
  /** @brief Edit an entry's stop date. */
  virtual void edit_entry_stop(Id entry_id,
                               const Date &new_stop_date) = 0;
  /** @brief Edit the entrystaging's project name. */
  virtual void edit_entrystaging_project_name
      (const std::string &new_project_name) = 0;
  /** @brief Edit the entrystaging's task name. */
  virtual void edit_entrystaging_task_name
      (const std::string &new_task_name) = 0;
  /** @brief Delete the task with provided Id. */
  virtual void delete_task (Id task_id) = 0;
  /** @brief Delete the project with provided Id and all associated tasks. */
  virtual void delete_project (Id project_id) = 0;
  /** @brief Delete a timesheet entry. */
  virtual void delete_entry (Id entry_id) = 0;
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
