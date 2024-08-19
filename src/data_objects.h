/** @file
 * @brief Shared definition of data objects in the program. */
#ifndef DATA_OBJECTS_H
#define DATA_OBJECTS_H

#include <string>
#include <cstdint>
#include "date.h"

/** @brief Unique id for data items (typically a SQLite rowid). */
typedef uint64_t Id;

/** @brief A generic item has an Id and a display name. */
struct GenericItem {
  /** @brief Globally unique Id. */
  Id id;
  /** @brief Name of the item. */
  std::string name;
};

/** @brief Specialization of GenericItem into a Project item. */
struct Project : GenericItem {};
/** @brief Specialization of GenericItem into a Task item. */
struct Task : GenericItem {};


/** @brief Timesheet entry object. */
struct Entry {
  Id id;
  std::string project_name;
  std::string task_name;
  Date start;
  Date stop;
};

#endif // DATA_OBJECTS_H
