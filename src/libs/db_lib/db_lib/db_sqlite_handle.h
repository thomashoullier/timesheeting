/** @file
 * @brief Low-level SQLite C++ wrapper. */
#ifndef DB_SQLITE_HANDLE_H
#define DB_SQLITE_HANDLE_H

#include "db_sqlite_connection.h"
#include "statement.h"
#include <filesystem>
#include <memory>
#include <string>

namespace db_lib {
  /** @brief Low-level SQLite wrapper handle and utility class. */
  class DB_SQLite_Handle {
  public:
    /** @brief Open or create the DB. */
    explicit DB_SQLite_Handle(const std::filesystem::path &db_file);

    /** @brief Check or initialize the user_version of the DB. */
    void check_user_version(int user_version);
    /** @brief Get user_version of the DB. */
    int get_user_version() const;
    /** @brief Prepare a SQL statement. */
    Statement prepare_statement(const std::string &statement);
    /** @brief Execute a SQL statement in the DB. */
    void exec_statement(const std::string &statement);

  private:
    /** @brief Pointer to the DB connection. Shared with statements prepared
        by this handle. */
    std::shared_ptr<DB_SQLite_Connection> db;

    /** @brief Check a SQLite return code and raise exception with message
        in case of reported errors. */
    void check_rc(int rc, const std::string &msg);
    /** @brief Set the user_version of the DB. */
    void set_user_version(int user_version);
  };
} // namespace db_lib
#endif // DB_SQLITE_HANDLE_H
