#ifndef DATA_OBJECTS_H
#define DATA_OBJECTS_H

#include <string>
#include <cstdint>

/** Defining various internal representations. */
typedef uint64_t Id; // Unique id for data items (typically a SQLite rowid).

/** A generic item has an Id and a display name. */
struct GenericItem {
  Id id; // Unique id.
  std::string name; // Display name.
};

struct Project : GenericItem {};
struct Task : GenericItem {};

#endif // DATA_OBJECTS_H
