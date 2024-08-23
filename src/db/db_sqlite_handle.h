/** @file
 * @brief Low-level SQLite C++ wrapper. */
#ifndef DB_SQLITE_HANDLE_H
#define DB_SQLITE_HANDLE_H

#include <exception>
#include <filesystem>
#include <string>
#include <cstdint>
#include <vector>
#include "sqlite3.h"

/** @brief C++ representation of a SQLite3 rowid. */
typedef uint64_t RowId;
/** @brief Rows of (RowId, Name) pairs. */
typedef std::vector<std::pair<RowId, std::string>> NameRows;

/** @brief Low-level SQLite wrapper handle and utility class. */
class DB_SQLite_Handle {
public:
  /** @brief Open or create the DB. */
  DB_SQLite_Handle (const std::filesystem::path &db_file);
  ~DB_SQLite_Handle ();

  /** @brief Prepare a SQL statement. */
  sqlite3_stmt* prepare_statement (const std::string &statement);
  /** @brief Execute a SQL statement in the DB. */
  void exec_statement (const std::string &statement);
  /** @brief Return the result of a statement querying a list of rows with
      (rowid, string), in order. */
  NameRows query_row_of_names (const std::string &statement);

private :
  /** @brief Internal pointer to the DB. */
  sqlite3 *db;
  /** @brief Check a SQLite return code and raise exception with message in case
   * of reported errors. */
  void check_rc (int rc, const std::string &msg);
};

/** @brief Exception thrown when encountering the SQLITE_CONSTRAINT error
 * code. */
class SQLiteConstraintExcept : public std::exception {
private:
  /** @brief Exception message. */
  std::string msg;

public:
  /** @brief Exception constructor. */
  SQLiteConstraintExcept (const char* _msg) : msg(_msg) {}
  /** @brief Mandatory std::exception method. */
  const char* what() const throw() {
    return msg.c_str();
  }
};

#endif // DB_SQLITE_HANDLE_H
