/** @file
    @brief Wrapper object around a connection to a sqlite3 DB. */
#ifndef DB_SQLITE_CONNECTION_H
#define DB_SQLITE_CONNECTION_H

#include <filesystem>
#include <sqlite3.h>

namespace db_lib {
  /** @brief Low-level SQLite wrapper around a DB connection. Not meant
      for direct use by the user. */
  class DB_SQLite_Connection {
  public:
    /** @brief Constructor. */
    explicit DB_SQLite_Connection(const std::filesystem::path &db_file);
    /** @brief Destructor. */
    ~DB_SQLite_Connection();
    /** @brief Internal pointer to the DB. */
    sqlite3 *db;
  };
} // namespace db_lib

#endif // DB_SQLITE_CONNECTION_H
