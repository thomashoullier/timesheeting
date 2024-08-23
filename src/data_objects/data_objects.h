/** @file
 * @brief Shared definition of data objects in the program. */
#ifndef DATA_OBJECTS_H
#define DATA_OBJECTS_H

#include <optional>
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

  /** @brief Generate an ordered set of strings for displaying the entry. */
  std::vector<std::string> to_strings() const {
    std::vector<std::string> display_strings(4);
    display_strings.at(0) = project_name;
    display_strings.at(1) = task_name;
    display_strings.at(2) = start.to_string();
    display_strings.at(3) = stop.to_string();
    return display_strings;
  };
};

/** @brief Entry as it is being staged. */
struct EntryStaging {
  std::optional<std::string> project_name;
  std::optional<std::string> task_name;
  std::optional<Date> start;
  std::optional<Date> stop;

  /** @brief Generate an ordered set of strings for displaying
      the entrystaging. */
  std::vector<std::string> to_strings() const {
    std::vector<std::string> display_strings(4);
    display_strings.at(0) = project_name.value_or(" ");
    display_strings.at(1) = task_name.value_or(" ");
    display_strings.at(2) = start.has_value() ? start.value().to_string() : " ";
    display_strings.at(3) = stop.has_value() ? stop.value().to_string() : " ";
    return display_strings;
  };
};

/** @brief Entry field type (as displayed in the menu). */
enum EntryField { project_name = 0, task_name = 1, start = 2, stop = 3 };

#endif // DATA_OBJECTS_H
