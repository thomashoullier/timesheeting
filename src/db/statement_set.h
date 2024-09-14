#ifndef STATEMENT_SET_H
#define STATEMENT_SET_H

#include "db_sqlite_handle.h"
#include "statement.h"
#include <memory>

class StatementSet {
public:
  /** @brief Initialize all of the statements. */
  explicit StatementSet(std::shared_ptr<DB_SQLite_Handle> sqlite_db);

  /** @brief Statement for querying the list of tasks for a given project. */
  Statement select_tasks;
};

#endif // STATEMENT_SET_H
