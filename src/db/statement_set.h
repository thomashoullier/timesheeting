#ifndef STATEMENT_SET_H
#define STATEMENT_SET_H

#include "db_sqlite_handle.h"
#include "statement.h"
#include <memory>

class StatementSet {
public:
  /** @brief Initialize all of the statements. */
  explicit StatementSet(std::shared_ptr<DB_SQLite_Handle> sqlite_db);

  /** @brief Statement for querying the projects list. */
  Statement select_projects;
  /** @brief Statement for querying the list of active projects. */
  Statement select_projects_active;
  /** @brief Statement for querying the list of tasks for a given project. */
  Statement select_tasks;
  /** @brief Statement for querying the list of active tasks for a given
      project. */
  Statement select_tasks_active;
  /** @brief Statement for querying the list of locations. */
  Statement select_locations;
  /** @brief Statement for querying the list of active locations. */
  Statement select_locations_active;
  /** @brief Statement for querying the list of entries over a given date
      range. */
  Statement select_entries;
  /** @brief Statement for querying the total duration of entries over
      a date range. */
  Statement select_duration;
  /** @brief Statement for querying the entry contained in entrystaging. */
  Statement select_entrystaging;
  /** @brief Statement for adding a project. */
  Statement insert_project;
  /** @brief Statement for adding a task. */
  Statement insert_task;
  /** @brief Statement for adding a location. */
  Statement insert_location;
  /** @brief Statement for editing a project's name. */
  Statement update_project_name;
  /** @brief Statement for editing a task's name. */
  Statement update_task_name;
  /** @brief Statement for editing a location's name. */
  Statement update_location_name;
  /** @brief Statement for toggling the location active flag. */
  Statement toggle_location_flag;
  /** @brief Statement for toggling the task active flag. */
  Statement toggle_task_flag;
  /** @brief Statement for toggling the project active flag. */
  Statement toggle_project_flag;
};

#endif // STATEMENT_SET_H
