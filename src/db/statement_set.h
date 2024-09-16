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

};

#endif // STATEMENT_SET_H
