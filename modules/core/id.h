#ifndef ID_H
#define ID_H

#include <cstdint>

namespace core {
  /** @brief Unique id for data items (typically a SQLite rowid). */
  typedef uint64_t Id;
}
#endif // ID_H