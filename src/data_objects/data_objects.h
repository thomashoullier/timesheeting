/** @file
 * @brief Shared definition of data objects in the program. */
#ifndef DATA_OBJECTS_H
#define DATA_OBJECTS_H

#include <chrono>
#include <iomanip>
#include <optional>
#include <sstream>
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
  /** @brief Id of the entry. */
  Id id;
  /** @brief Project name associated to the entry. */
  std::string project_name;
  /** @brief Task name associated to the entry. */
  std::string task_name;
  /** @brief Start date of the entry. */
  Date start;
  /** @brief Stop date of the entry. */
  Date stop;

  /** @brief Generate an ordered set of strings for displaying the entry.

   Typically, the result is like {"Project", "Task",
  "23Aug2024 16:24:05", "23Aug2024 17:35:38"}*/
  std::vector<std::string> to_strings() const {
    std::vector<std::string> display_strings(4);
    display_strings.at(0) = project_name;
    display_strings.at(1) = task_name;
    display_strings.at(2) = start.to_string();
    display_strings.at(3) = stop.to_string();
    return display_strings;
  };
};

/** @brief Entry as it is being staged.

 Each field may or may not be currently filled. */
struct EntryStaging {
  /** @brief Project name associated to the entry in staging. */
  std::optional<std::string> project_name;
  /** @brief Task name associated to the entry in staging. */
  std::optional<std::string> task_name;
  /** @brief Start date of the entry in staging. */
  std::optional<Date> start;
  /** @brief Stop date of the entry in staging. */
  std::optional<Date> stop;

  /** @brief Generate an ordered set of strings for displaying
      the entrystaging.

  Fields with no values are output as a whitespace character. */
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

/** @brief Duration type*/
class Duration {
private:
  /** @brief Duration represented as a number of seconds. */
  std::chrono::seconds dur;

public:
  /** @brief Initialize the duration to a number of seconds. */
  Duration (uint64_t seconds) :
    dur(std::chrono::seconds(seconds)) {};
  /** @brief Obtain a displayable representation for the duration in
      hours with 3 decimal places. */
  std::string to_string() const {
    double hours = static_cast<double>(dur.count()) / 3600;
    std::ostringstream ostr;
    ostr << std::fixed << std::setprecision(3)
         << hours << " hours";
    return ostr.str();
  };
};

#endif // DATA_OBJECTS_H
