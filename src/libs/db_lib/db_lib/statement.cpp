#include "statement.h"
#include "db_sqlite_handle.h"
#include <stdexcept>

namespace db_lib {
  Statement::Statement(sqlite3_stmt *_stmt) : stmt(_stmt) {}

  Statement::~Statement() { sqlite3_finalize(stmt); }

  bool Statement::check_rc(int rc) {
    switch (rc) {
    case SQLITE_OK:
      return true;
    case SQLITE_DONE: // Emitted when a step is finished.
      return true;
    case SQLITE_CONSTRAINT:
      return false;
    default:
      throw std::runtime_error
        ("Unexpected SQLite return code: " + std::to_string(rc));
    }
  }

  bool Statement::step() {
    auto rc = sqlite3_step(stmt);
    if (not(rc == SQLITE_ROW)) {
      sqlite3_reset(stmt);
      return false;
    }
    return true;
  }

  bool Statement::execute() {
    auto rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
    return check_rc(rc);
  }

  void Statement::bind(int index, DBInt num) {
    sqlite3_bind_int64(stmt, index + 1, num);
  }

  void Statement::bind(int index, const std::string &str) {
    sqlite3_bind_text(stmt, index + 1, str.c_str(), str.size(), SQLITE_STATIC);
  }

  /** @brief Specialization for getting an integer. */
  template <> DBInt Statement::get_column(int icol) {
    return sqlite3_column_int64(stmt, icol);
  }

  /** @brief Specialization for getting a boolean.

   A boolean in ncurses is actually an INTEGER. */
  template <> bool Statement::get_column(int icol) {
    return sqlite3_column_int(stmt, icol);
  }

  /** @brief Specialization for getting std::string.

      In case the column returns a NULL char pointer, then we return
      an empty string as a default. */
  template <> std::string Statement::get_column(int icol) {
    auto text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, icol));
    return (text == NULL ? std::string{} : text);
  }
}
