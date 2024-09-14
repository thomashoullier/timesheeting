#include "statement.h"
#include "db_sqlite_handle.h"

Statement::Statement(sqlite3_stmt *_stmt) : stmt(_stmt) {}

Statement::~Statement() { sqlite3_finalize(stmt); }
