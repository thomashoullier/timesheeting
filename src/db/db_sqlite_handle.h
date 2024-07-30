/** @file Low-level SQLite C++ wrapper class. */
#ifndef DB_SQLITE_HANDLE_H
#define DB_SQLITE_HANDLE_H

#include <exception>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <vector>
#include "sqlite3.h"

/** C++ representation of a SQLite3 rowid. */
typedef uint64_t RowId;

/** Rows of (RowId, Name) pairs. */
typedef std::vector<std::pair<RowId, std::string>> NameRows;

class DB_SQLite_Handle {
public:
  DB_SQLite_Handle (const std::filesystem::path &db_file);
  ~DB_SQLite_Handle ();

  void exec_statement (const std::string &statement);

  /** Return the result of a statement querying a list of rows with
      (rowid, string), in order. */
  NameRows query_row_of_names (const std::string &statement);

private :
  sqlite3 *db;
  /** Check a SQLite return code and raise exception with message in
      case of reported errors. */
  void check_rc (int rc, const std::string &msg);
};

/** Exception thrown when encountering the SQLITE_CONSTRAINT error code.*/
class SQLiteConstraintExcept : public std::exception {
private:
  std::string msg; // Message with the exception.

public:
  SQLiteConstraintExcept (const char* _msg) : msg(_msg) {}
  const char* what() const throw() {
    return msg.c_str();
  }
};

#endif // DB_SQLITE_HANDLE_H
