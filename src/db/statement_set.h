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
  /** @brief Statement for querying the export data for entries. */
  Statement select_export_entries;
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
  /** @brief Statement for editing an entry's project. */
  Statement update_entry_project;
  /** @brief Statement for editing an entry's task. */
  Statement update_entry_task;
  /** @brief Statement for editing an entry's start date. */
  Statement update_entry_start;
  /** @brief Statement for editing an entry's stop date. */
  Statement update_entry_stop;
  /** @brief Statement for editing an entry's location. */
  Statement update_entry_location;
  /** @brief Statement for editing the entrystaging project name. */
  Statement update_entrystaging_project;
  /** @brief Statement for editing the entrystaging task name. */
  Statement update_entrystaging_task;
  /** @brief Statement for editing the entrystaging start date. */
  Statement update_entrystaging_start;
  /** @brief Statement for editing the entrystaging stop date. */
  Statement update_entrystaging_stop;
  /** @brief Statement for editing the entrystaging location. */
  Statement update_entrystaging_location;
  /** @brief Statement for deleting a task. */
  Statement remove_task;
  /** @brief Statement for deleting a project. */
  Statement remove_project;
  /** @brief Statement for deleting a location. */
  Statement remove_location;
  /** @brief Statement for deleting an entry. */
  Statement remove_entry;
  /** @brief Statement for committing the entrystaging to entries. */
  Statement insert_entrystaging;
  /** @brief Sum duration per project for a given date range. */
  Statement sum_duration_per_project;
  /** @brief Sum duration for a given project over a given date range. */
  Statement project_duration;
  /** @brief Sum duration for a given task over a given date range. */
  Statement task_duration;
  /** @brief Sum duration per project actually worked on
      for a given date range. */
  Statement duration_per_worked_project;
  /** @brief Sum duration per task actually worked on for a given date range,
      on a given project. */
  Statement duration_per_worked_task;
};

#endif // STATEMENT_SET_H
