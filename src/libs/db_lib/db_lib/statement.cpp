#include "statement.h"
#include "db_types.h"
#include <stdexcept>
#include <string>

namespace db_lib {
  Statement::Statement(sqlite3_stmt *_stmt) : stmt(_stmt) {}

  Statement::~Statement() { sqlite3_finalize(stmt); }

  bool Statement::check_rc(int rc) {
    switch (rc) {
    case SQLITE_OK:
      return true;
    case SQLITE_DONE: // Emitted when a step is finished.
      return true;
    case SQLITE_ROW:
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
    check_rc(rc);
    if (not(rc == SQLITE_ROW)) {
      rc = sqlite3_reset(stmt);
      check_rc(rc);
      return false;
    }
    return true;
  }

  bool Statement::execute() {
    auto rc_step = sqlite3_step(stmt);
    check_rc(rc_step);
    auto rc_reset = sqlite3_reset(stmt);
    check_rc(rc_reset);
    return check_rc(rc_step);
  }

  void Statement::bind(int index, DBInt num) {
    auto rc = sqlite3_bind_int64(stmt, index + 1, num);
    check_rc(rc);
  }

  void Statement::bind(int index, const std::string &str) {
    auto rc = sqlite3_bind_text(stmt, index + 1, str.c_str(),
                                str.size(), SQLITE_STATIC);
    check_rc(rc);
  }

  /** @brief Specialization for getting an integer. */
  template <> DBInt Statement::get_column(int icol) {
    if (sqlite3_data_count(stmt) == 0)
      throw std::runtime_error("Statement has no data to return.");
    auto ncols = sqlite3_column_count(stmt);
    if (ncols <= icol)
      throw std::runtime_error("Asked to retrieve column of index " +
                               std::to_string(icol) +
                               " when the statement row " + "only has " +
                               std::to_string(ncols) + " columns.");
    auto column_type = sqlite3_column_type(stmt, icol);
    if (column_type == SQLITE_NULL)
      return 0;
    if (column_type != SQLITE_INTEGER)
      throw std::runtime_error("Tried to get a SQLITE_INTEGER but got a "
                               "type code: " + std::to_string(column_type));
    return sqlite3_column_int64(stmt, icol);
  }

  /** @brief Specialization for getting a boolean.

   A boolean in ncurses is actually an INTEGER. */
  template <> bool Statement::get_column(int icol) {
    if (sqlite3_data_count(stmt) == 0)
      throw std::runtime_error("Statement has no data to return.");
    auto ncols = sqlite3_column_count(stmt);
    if (ncols <= icol)
      throw std::runtime_error("Asked to retrieve column of index " +
                               std::to_string(icol) +
                               " when the statement row " + "only has " +
                               std::to_string(ncols) + " columns.");
    auto column_type = sqlite3_column_type(stmt, icol);
    if (column_type == SQLITE_NULL)
      return 0;
    if (column_type != SQLITE_INTEGER)
      throw std::runtime_error("Tried to get a SQLITE_INTEGER (bool) but got a "
                               "type code: " +
                               std::to_string(column_type));
    return sqlite3_column_int(stmt, icol);
  }

  /** @brief Specialization for getting std::string.

      In case the column returns a NULL char pointer, then we return
      an empty string as a default. */
  template <> std::string Statement::get_column(int icol) {
    if (sqlite3_data_count(stmt) == 0)
      throw std::runtime_error("Statement has no data to return.");
    auto ncols = sqlite3_column_count(stmt);
    if (ncols <= icol)
      throw std::runtime_error("Asked to retrieve column of index " +
                               std::to_string(icol) +
                               " when the statement row " + "only has " +
                               std::to_string(ncols) + " columns.");
    auto column_type = sqlite3_column_type(stmt, icol);
    if (column_type == SQLITE_NULL)
      return {};
    if (column_type != SQLITE_TEXT)
      throw std::runtime_error("Tried to get a SQLITE_TEXT but got a "
                               "type code: " + std::to_string(column_type));
    auto text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, icol));
    // TODO: check for possible out-of-memory error with sqlite3_errcode()
    //       Requires handle to the DB.
    return text;
  }
}
