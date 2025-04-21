/** @file
 *  @brief Id definition. */
#ifndef ID_H
#define ID_H

#include "db_lib/db_types.h"

namespace core {
  /** @brief Unique id for data items. This is simply the DB row id. */
  typedef db_lib::RowId Id;
}
#endif // ID_H
