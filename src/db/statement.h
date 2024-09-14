#ifndef STATEMENT_H
#define STATEMENT_H

#include "db_sqlite_handle.h"

/** @brief Statement object. */
class Statement {
private:
  /** @brief Bind an uint64_t to a parameter of the statement. */
  void bind(int index, uint64_t num);

public:
  /** @brief SQLite statement. */
  sqlite3_stmt *stmt; // TODO: make it private when interface is complete.
  /** @brief Constructor. */
  Statement(sqlite3_stmt *_stmt);
  /** @brief Destructor. */
  ~Statement();

  /** @brief Bind all arguments to the statement. */
  template <class... T>
  void bind_all(const T&... values) {
    int index = 0;
    ([&]
    {
      bind(index, values);
      ++index;
    } (), ...);
  };
};

#endif // STATEMENT_H
