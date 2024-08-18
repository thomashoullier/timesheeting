#include "db_sqlite_handle.h"

DB_SQLite_Handle::DB_SQLite_Handle(const std::filesystem::path &db_file) {
  auto rc = sqlite3_open(db_file.c_str(), &db);
  check_rc(rc, "Error when opening the database file: " + std::string(db_file));
  exec_statement("PRAGMA foreign_keys = ON;");
}

DB_SQLite_Handle::~DB_SQLite_Handle() { sqlite3_close(db); }

void DB_SQLite_Handle::check_rc(int rc, const std::string &msg) {
  switch (rc) {
  case SQLITE_OK:
    return;
  case SQLITE_CONSTRAINT:
    throw SQLiteConstraintExcept(msg.c_str());
      break;
  default:
    throw std::runtime_error(msg + "\n" +
                             "SQLite error code: " + std::to_string(rc));
    break;
  }
}

sqlite3_stmt *DB_SQLite_Handle::prepare_statement(const std::string &statement) {
  sqlite3_stmt *stmt;
  auto rc = sqlite3_prepare_v2(db, statement.c_str(), -1, &stmt, NULL);
  check_rc(rc, "Could not prepare SQL statement: " + statement);
  return stmt;
}

void DB_SQLite_Handle::exec_statement(const std::string &statement) {
  auto rc = sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL);
  check_rc(rc, "Error when executing statement: " + statement);
}

NameRows DB_SQLite_Handle::query_row_of_names(const std::string &statement) {
  auto stmt = prepare_statement(statement);
  NameRows rows {};
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    RowId id = sqlite3_column_int64(stmt, 0);
    auto name_internal = sqlite3_column_text(stmt, 1);
    std::string name = reinterpret_cast<const char*>(name_internal);
    rows.push_back(std::make_pair(id, name));
  }
  sqlite3_finalize(stmt);
  return rows;
}
