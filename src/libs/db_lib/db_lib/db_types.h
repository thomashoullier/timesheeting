/** @file
    @brief Types for the DB library. */
#ifndef DB_TYPES_H
#define DB_TYPES_H

#include <sqlite3.h>

namespace db_lib {
  /** @brief Type for integers in the DB. */
  typedef sqlite3_int64 DBInt;
  /** @brief Type for a DB rowid. */
  typedef DBInt RowId;
}

#endif // DB_TYPES_H
