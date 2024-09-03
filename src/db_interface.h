/** @file
 * @brief DB virtual interface definition. */
#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <exception>
#include <string>
#include "data_objects/data_objects.h"
#include "data_objects/date_range.h"
#include "data_objects/project_total.h"

/** @brief Virtual interface class for database. */
class DB_Interface {
public:
  virtual ~DB_Interface() = default;
  /** @brief Return the set of projects stored in the DB. */
  virtual std::vector<Project> query_projects() = 0;
  /** @brief Return the set of tasks for a given project. */
  virtual std::vector<Task> query_tasks (Id project_id) = 0;
  /** @brief Return the set of active tasks for a given project. */
  virtual std::vector<Task> query_tasks_active (Id project_id) = 0;
  /** @brief Return the set of locations stored in the DB. */
  virtual std::vector<Location> query_locations () = 0;
  /** @brief Return the set of active locations. */
  virtual std::vector<Location> query_locations_active () = 0;
  /** @brief Return the set of entries ordered by ascending start date
      and included in the provided DateRange. */
  virtual std::vector<Entry> query_entries (const DateRange &date_range) = 0;
  /** @brief Return the total duration of entries
      starting within the date range. */
  virtual Duration query_entries_duration(const DateRange &date_range) = 0;
  /** @brief Get the displayable state of the Entry in staging. */
  virtual EntryStaging query_entrystaging () = 0;

  /** @brief Add a project to the DB with the provided name. */
  virtual void add_project(std::string project_name) = 0;
  /** @brief Add a task to a given project with the provided task name. */
  virtual void add_task(Id project_id, std::string task_name) = 0;
  /** @brief Add a location to the DB with the provided name. */
  virtual void add_location(const std::string &location_name) = 0;
  /** @brief Add an entry for a given task with a start and stop dates. */
  virtual void add_entry(Id task_id, const Date &start, const Date &stop,
                         Id location_id) = 0;
  /** @brief Edit a project's name. */
  virtual void edit_project_name(Id project_id,
                                 std::string new_project_name) = 0;
  /** @brief Edit a task's name. */
  virtual void edit_task_name(Id task_id,
                              std::string new_task_name) = 0;
  /** @brief Edit a location's name. */
  virtual void edit_location_name(Id location_id,
                                  const std::string &new_location_name) = 0;
  /** @brief Toggle the location active state. */
  virtual void toggle_location_active(Id location_id) = 0;
  /** @brief Toggle a task active state. */
  virtual void toggle_task_active(Id task_id) = 0;
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
  /** @brief Edit an entry's location. */
  virtual void edit_entry_location(Id entry_id,
                                   const std::string &new_location_name) = 0;
  /** @brief Edit the entrystaging's project name. */
  virtual void edit_entrystaging_project_name
      (const std::string &new_project_name) = 0;
  /** @brief Edit the entrystaging's task name. */
  virtual void edit_entrystaging_task_name
      (const std::string &new_task_name) = 0;
  /** @brief Edit the entrystaging's start date. */
  virtual void edit_entrystaging_start (const Date &new_start) = 0;
  /** @brief Edit the entrystaging's stop date. */
  virtual void edit_entrystaging_stop (const Date &new_stop) = 0;
  /** @brief Edit the entrystaging's location with an active location. */
  virtual void edit_entrystaging_location_name
      (const std::string &new_location_name) = 0;
  /** @brief Delete the task with provided Id. */
  virtual void delete_task (Id task_id) = 0;
  /** @brief Delete the project with provided Id and all associated tasks. */
  virtual void delete_project (Id project_id) = 0;
  /** @brief Delete the location with provided Id. */
  virtual void delete_location (Id location_id) = 0;
  /** @brief Delete a timesheet entry. */
  virtual void delete_entry (Id entry_id) = 0;
  /** @brief Commit the entrystaging to the entries table. */
  virtual void commit_entrystaging () = 0;
  /** @brief Report the total duration worked per project
      over given date range. */
  virtual std::vector<ProjectTotal> report_project_totals
  (const DateRange &date_range) = 0;
};

/** @brief Exception thrown when the DB encounters a logic error. */
class DBLogicExcept : public std::exception {
private:
  /** @brief Exception message. */
  std::string msg;

public:
  /** @brief Exception constructor. */
  DBLogicExcept (const char* _msg) : msg(_msg) {}
  /** @brief Mandatory std::exception method. */
  const char* what() const throw() {
    return msg.c_str();
  }
};

#endif // DB_INTERFACE_H
