/** @file
 *  @brief Defining all repeatedly used DB statements for the program. */
#ifndef STATEMENT_SET_H
#define STATEMENT_SET_H

#include "db_lib/db_sqlite_handle.h"
#include "db_lib/statement.h"
#include <memory>

namespace db {
  /** @brief Set of DB statements which will be used repeatedly
             in the program. */
  class StatementSet {
  public:
    /** @brief Initialize all of the statements. */
    explicit StatementSet(std::shared_ptr<db_lib::DB_SQLite_Handle> sqlite_db);

    /** @brief db_lib::Statement for querying the projects list. */
    db_lib::Statement select_projects;
    /** @brief db_lib::Statement for querying the list of active projects. */
    db_lib::Statement select_projects_active;
    /** @brief db_lib::Statement for querying the list of tasks
        for a given project. */
    db_lib::Statement select_tasks;
    /** @brief db_lib::Statement for querying the list of active tasks
        for a given project. */
    db_lib::Statement select_tasks_active;
    /** @brief db_lib::Statement for querying the list of locations. */
    db_lib::Statement select_locations;
    /** @brief db_lib::Statement for querying the list of active locations. */
    db_lib::Statement select_locations_active;
    /** @brief db_lib::Statement for querying the list of entries
        over a given date range. */
    db_lib::Statement select_entries;
    /** @brief db_lib::Statement to return the project id for an entry. */
    db_lib::Statement select_entry_project_id;
    /** @brief db_lib::Statement for querying the export data for entries. */
    db_lib::Statement select_export_entries;
    /** @brief db_lib::Statement for querying the total duration
        of entries over a date range. */
    db_lib::Statement select_duration;
    /** @brief db_lib::Statement for querying the entry contained
        in entrystaging. */
    db_lib::Statement select_entrystaging;
    /** @brief db_lib::Statement for querying the project id in entrystaging. */
    db_lib::Statement select_entrystaging_project_id;
    /** @brief db_lib::Statement for adding a project. */
    db_lib::Statement insert_project;
    /** @brief db_lib::Statement for adding a task. */
    db_lib::Statement insert_task;
    /** @brief db_lib::Statement for adding a location. */
    db_lib::Statement insert_location;
    /** @brief db_lib::Statement for editing a project's name. */
    db_lib::Statement update_project_name;
    /** @brief db_lib::Statement for editing a task's name. */
    db_lib::Statement update_task_name;
    /** @brief db_lib::Statement for editing a task's project. */
    db_lib::Statement update_task_project;
    /** @brief db_lib::Statement for editing a location's name. */
    db_lib::Statement update_location_name;
    /** @brief db_lib::Statement for toggling the location active flag. */
    db_lib::Statement toggle_location_flag;
    /** @brief db_lib::Statement for toggling the task active flag. */
    db_lib::Statement toggle_task_flag;
    /** @brief db_lib::Statement for toggling the project active flag. */
    db_lib::Statement toggle_project_flag;
    /** @brief db_lib::Statement for editing an entry's project. */
    db_lib::Statement update_entry_project;
    /** @brief db_lib::Statement for editing an entry's task. */
    db_lib::Statement update_entry_task;
    /** @brief db_lib::Statement for editing an entry's start date. */
    db_lib::Statement update_entry_start;
    /** @brief db_lib::Statement for editing an entry's stop date. */
    db_lib::Statement update_entry_stop;
    /** @brief db_lib::Statement for editing an entry's location. */
    db_lib::Statement update_entry_location;
    /** @brief db_lib::Statement for editing the entrystaging project name. */
    db_lib::Statement update_entrystaging_project;
    /** @brief db_lib::Statement for editing the entrystaging task name. */
    db_lib::Statement update_entrystaging_task;
    /** @brief db_lib::Statement for editing the entrystaging start date. */
    db_lib::Statement update_entrystaging_start;
    /** @brief db_lib::Statement for editing the entrystaging stop date. */
    db_lib::Statement update_entrystaging_stop;
    /** @brief db_lib::Statement for editing the entrystaging location. */
    db_lib::Statement update_entrystaging_location;
    /** @brief db_lib::Statement for deleting a task. */
    db_lib::Statement remove_task;
    /** @brief db_lib::Statement for deleting a project. */
    db_lib::Statement remove_project;
    /** @brief db_lib::Statement for deleting a location. */
    db_lib::Statement remove_location;
    /** @brief db_lib::Statement for deleting an entry. */
    db_lib::Statement remove_entry;
    /** @brief db_lib::Statement for committing the entrystaging to entries. */
    db_lib::Statement insert_entrystaging;
    /** @brief Sum duration per project for a given date range. */
    db_lib::Statement sum_duration_per_project;
    /** @brief Sum duration for a given project over a given date range. */
    db_lib::Statement project_duration;
    /** @brief Sum duration for a given task over a given date range. */
    db_lib::Statement task_duration;
    /** @brief Sum duration per project actually worked on
        for a given date range. */
    db_lib::Statement duration_per_worked_project;
    /** @brief Sum duration per task actually worked on for a given date range,
        on a given project. */
    db_lib::Statement duration_per_worked_task;
  };
}
#endif // STATEMENT_SET_H
