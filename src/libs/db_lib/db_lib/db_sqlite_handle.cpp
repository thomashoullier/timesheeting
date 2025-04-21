#include "db_sqlite_handle.h"
#include "db_sqlite_connection.h"
#include <memory>
#include <stddef.h>
#include <stdexcept>
#include <string>

namespace db_lib{
  DB_SQLite_Handle::DB_SQLite_Handle(const std::filesystem::path &db_file) :
    db {std::make_shared<DB_SQLite_Connection>(db_file)} {
    exec_statement("PRAGMA foreign_keys = ON;");
    exec_statement("PRAGMA optimize;");
  }

  void DB_SQLite_Handle::check_rc(int rc, const std::string &msg) {
    switch (rc) {
    case SQLITE_OK:
      return;
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
    sqlite3_prepare_v2(db->db, statement.c_str(), statement.size(), &stmt, NULL);
    sqlite3_step(stmt);
    int user_version = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return user_version;
  }

  sqlite3_stmt *DB_SQLite_Handle::prepare_statement
    (const std::string &statement) {
    sqlite3_stmt *stmt;
    auto rc = sqlite3_prepare_v3(db->db, statement.c_str(), statement.size(),
                                 SQLITE_PREPARE_PERSISTENT, &stmt, NULL);
    check_rc(rc, "Could not prepare SQL statement: " + statement);
    return stmt;
  }

  void DB_SQLite_Handle::exec_statement(const std::string &statement) {
    auto rc = sqlite3_exec(db->db, statement.c_str(), NULL, NULL, NULL);
    check_rc(rc, "Error when executing statement: " + statement);
  }
} // namespace db_lib
