#include "db_sqlite_handle.h"
#include <string>

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
  case SQLITE_DONE: // Emitted when a step is finished.
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

void DB_SQLite_Handle::check_user_version(int user_version) {
  auto db_user_version = get_user_version();
  if (db_user_version == 0) { // Initialize new DB
    set_user_version(user_version);
  } else if (db_user_version == user_version) { // Match.
    return;
  } else { // Version mismatch
    throw std::runtime_error("DB version mismatch, expected v"
                             + std::to_string(user_version) + " but got v"
                             + std::to_string(db_user_version));
  }
}

void DB_SQLite_Handle::set_user_version(int user_version) {
  // NOTE: it is not possible to use a parameterized PRAGMA in SQLite.
  exec_statement("PRAGMA user_version = " + std::to_string(user_version)
                 + ";");
}

int DB_SQLite_Handle::get_user_version() const {
  const std::string statement{"PRAGMA user_version;"};
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, statement.c_str(), statement.size(), &stmt, NULL);
  sqlite3_step(stmt);
  int user_version = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  return user_version;
}

sqlite3_stmt *DB_SQLite_Handle::prepare_statement(
    const std::string &statement) {
  sqlite3_stmt *stmt;
  //auto rc = sqlite3_prepare_v2(db, statement.c_str(), -1, &stmt, NULL);
  auto rc = sqlite3_prepare_v3(db, statement.c_str(), statement.size(),
                               SQLITE_PREPARE_PERSISTENT, &stmt, NULL);
  check_rc(rc, "Could not prepare SQL statement: " + statement);
  return stmt;
}

void DB_SQLite_Handle::exec_statement(const std::string &statement) {
  auto rc = sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL);
  check_rc(rc, "Error when executing statement: " + statement);
}

void DB_SQLite_Handle::step_statement(sqlite3_stmt *stmt) {
  auto rc = sqlite3_step(stmt);
  check_rc(rc, "Error when stepping statment. ");
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
