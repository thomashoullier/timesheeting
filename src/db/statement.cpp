#include "statement.h"
#include "db_sqlite_handle.h"

Statement::Statement(sqlite3_stmt *_stmt) : stmt(_stmt) {}

Statement::~Statement() { sqlite3_finalize(stmt); }

bool Statement::step() {
  auto rc = sqlite3_step(stmt);
  if (not(rc == SQLITE_ROW)) {
    sqlite3_reset(stmt);
    return false;
  }
  return true;
}

void Statement::bind(int index, uint64_t num) {
  sqlite3_bind_int64(stmt, index + 1, num);
}

template <> uint64_t Statement::get_column(int icol) {
  return sqlite3_column_int64(stmt, icol);
}

template <> std::string Statement::get_column(int icol) {
  auto text = sqlite3_column_text(stmt, icol);
  return reinterpret_cast<const char*>(text);
}
