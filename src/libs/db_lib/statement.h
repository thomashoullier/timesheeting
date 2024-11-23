/** @file
 *  @brief Statement definition. */
#ifndef STATEMENT_H
#define STATEMENT_H

#include "db_sqlite_handle.h"
#include <stdexcept>
#include <tuple>
#include <utility>

namespace db_lib {
  /** @brief Statement object. */
  class Statement {
  private:
    /** @brief Bind an uint64_t to a parameter of the statement. */
    void bind(int index, uint64_t num);
    /** @brief Bind a string to a parameter of the statement.

        Note the string lifetime must be managed at the higher level.
    */
    void bind(int index, const std::string &str);
    /** @brief Retrieve a result from the current row in a given column. */
    template <typename T> T get_column(int icol);
    /** @brief Check the error code of the statement.

        Return true if the return code expresses a success, false if this is
        a constraint violation or otherwise expected failure, throw if the code
        is unexpected. */
    bool check_rc(int rc);

  public:
    /** @brief SQLite statement. */
    sqlite3_stmt *stmt; // TODO: make it private when interface is complete.
    /** @brief Constructor. */
    explicit Statement(sqlite3_stmt *_stmt);
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
    template <class... T>
    std::tuple<T...> get_all();
    /** @brief Helper for get_all(). Associates arguments with their
        index.*/
    template <class... T, std::size_t... Is>
    std::tuple<T...> get_all_helper(std::index_sequence<Is...>);
    /** @brief Get the results of all columns in the case of a single row.

        Step, get the results, and then reset.
        Return the default tuple values in case no data is available. */
    template <class... T>
    std::tuple<T...> single_get_all();
  };

  template <class... T>
  void Statement::bind_all(const T &...values) {
    // TODO: Use an index_sequence for compile time index, and uniform way
    //       of doing things.
    int index = 0;
    ([&] {
      bind(index, values);
      ++index;
    }(), ...);
  };

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
