/** @file
 * @brief Low-level SQLite C++ wrapper. */
#ifndef DB_SQLITE_HANDLE_H
#define DB_SQLITE_HANDLE_H

#include <exception>
#include <filesystem>
#include <string>
#include <vector>
#include <utility>
#include <sqlite3.h>

namespace db_lib {
  /** @brief Type for integers in the DB. */
  typedef sqlite3_int64 DBInt;
  /** @brief Type for a DB rowid. */
  typedef DBInt RowId;
  /** @brief Rows of (RowId, Name) pairs. */
  typedef std::vector<std::pair<RowId, std::string>> NameRows;

  /** @brief Low-level SQLite wrapper handle and utility class. */
  class DB_SQLite_Handle {
  public:
    /** @brief Open or create the DB. */
    explicit DB_SQLite_Handle(const std::filesystem::path &db_file);
    ~DB_SQLite_Handle();

    /** @brief Check or initialize the user_version of the DB. */
    void check_user_version(int user_version);
    /** @brief Get user_version of the DB. */
    int get_user_version() const;
    /** @brief Prepare a SQL statement. */
    sqlite3_stmt *prepare_statement(const std::string &statement);
    /** @brief Execute a SQL statement in the DB. */
    void exec_statement(const std::string &statement);

  private:
    /** @brief Internal pointer to the DB. */
    sqlite3 *db;
    /** @brief Check a SQLite return code and raise exception with message
        in case of reported errors. */
    void check_rc(int rc, const std::string &msg);
    /** @brief Set the user_version of the DB. */
    void set_user_version(int user_version);
  };

  /** @brief Exception thrown when encountering the SQLITE_CONSTRAINT error
   * code. */
  class SQLiteConstraintExcept : public std::exception {
  private:
    /** @brief Exception message. */
    std::string msg;

  public:
    /** @brief Exception constructor. */
    explicit SQLiteConstraintExcept (const char* _msg) : msg(_msg) {}
    /** @brief Mandatory std::exception method. */
    const char* what() const throw() {
      return msg.c_str();
    }
  };
} // namespace db_lib
#endif // DB_SQLITE_HANDLE_H
