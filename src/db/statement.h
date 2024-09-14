#ifndef STATEMENT_H
#define STATEMENT_H

#include "db_sqlite_handle.h"

/** @brief Statement object. */
class Statement {
private:

public:
  /** @brief SQLite statement. */
  sqlite3_stmt *stmt; // TODO: make it private when interface is complete.
  /** @brief Constructor. */
  Statement(sqlite3_stmt *_stmt);
  /** @brief Destructor. */
  ~Statement();
};

#endif // STATEMENT_H
