#include "db_sqlite_connection.h"
#include <sqlite3.h>
#include <stdexcept>
#include <string>

namespace db_lib {
  DB_SQLite_Connection::DB_SQLite_Connection
  (const std::filesystem::path &db_file) {
    auto rc = sqlite3_open(db_file.c_str(), &db);
    if (rc != SQLITE_OK) {
      sqlite3_close(db);
      throw std::runtime_error
        ("DB_SQLite_Connection, error when opening the DB file " +
         std::string(db_file) + " error code: " + std::to_string(rc));
    }
  }

  DB_SQLite_Connection::~DB_SQLite_Connection() { sqlite3_close(db); }
} // namespace db_lib
