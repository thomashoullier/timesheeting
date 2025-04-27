/** @file
 *  @brief Statement definition. */
#ifndef STATEMENT_H
#define STATEMENT_H

#include "db_sqlite_connection.h"
#include "db_types.h"
#include <memory>
#include <sqlite3.h>
#include <cstddef>
#include <string>
#include <tuple>
// IWYU pragma: no_include <variant>
#include <utility>

namespace db_lib {
  /** @brief Statement object. */
  class Statement {
  private:
    /** @brief Pointer to the DB the statement was prepared in. */
    std::shared_ptr<DB_SQLite_Connection> db;
    /** @brief SQLite statement. */
    sqlite3_stmt *stmt;

    /** @brief Bind a DB integer to a parameter of the statement. */
    void bind(int index, DBInt num);
    /** @brief Bind a string to a parameter of the statement.

        Note the string lifetime must be managed at the higher level.
        A string literal is not a valid input. */
    void bind(int index, const std::string &str);
    /** @brief Retrieve a result from the current row in a given column. */
    template <typename T> T get_column(int icol);
    /** @brief Check the error code of the statement.

        Return true if the return code expresses a success, false if this is
        a constraint violation or otherwise expected failure, throw if the code
        is unexpected. */
    bool check_rc(int rc);
    /** @brief Helper for get_all(). Associates arguments with their
        index.*/
    template <class... T, std::size_t... Is>
    std::tuple<T...> get_all_helper(std::index_sequence<Is...>);

  public:
    /** @brief Constructor. */
    explicit Statement(sqlite3_stmt *_stmt,
                       std::shared_ptr<DB_SQLite_Connection> _db);
    /** @brief Destructor. */
    ~Statement();

    /**@brief Try to step the statement. Reset if it has finished.
       Return true if there is a result available. */
    bool step();
    /** @brief Execute a statement (single use), and reset immediately.
        Return true on success, false on failure. */
    bool execute();

    /** @brief Bind all arguments to the statement. */
    template <class... T> void bind_all(const T &...values);
    /** @brief Get the results of all columns. */
    template <class... T> std::tuple<T...> get_all();
    /** @brief Get the results of all columns in the case of a single row.

        Step, get the results, and then reset.
        Return the default tuple values in case no data is available. */
    template <class... T> std::tuple<T...> single_get_all();
  };

  template <class... T>
  void Statement::bind_all(const T &...values) {
    int index = -1;
    (bind(++index, values), ...);
  }

  template <class... T>
  std::tuple<T...> Statement::get_all() {
    return get_all_helper<T...>(std::index_sequence_for<T...>{});
  }

  template <class... T, std::size_t... Is>
  std::tuple<T...> Statement::get_all_helper(std::index_sequence<Is...>) {
    return std::make_tuple(get_column<T>(Is)...);
  }

  template <class... T>
  std::tuple<T...> Statement::single_get_all() {
    auto results = step() ? get_all<T...>() : std::make_tuple(T{}...);
    sqlite3_reset(stmt);
    return results;
  }
}
#endif // STATEMENT_H
