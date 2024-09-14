#include "statement.h"
#include "db_sqlite_handle.h"

Statement::Statement(sqlite3_stmt *_stmt) : stmt(_stmt) {}

Statement::~Statement() { sqlite3_finalize(stmt); }

void Statement::bind(int index, uint64_t num) {
  sqlite3_bind_int64(stmt, index + 1, num);
}
