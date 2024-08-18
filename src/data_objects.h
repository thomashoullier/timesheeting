/** @file
 * @brief Shared definition of data objects in the program. */
#ifndef DATA_OBJECTS_H
#define DATA_OBJECTS_H

#include <string>
#include <cstdint>
#include <chrono>
#include <format>

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

/** @brief Date type. */
class Date {
private:
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp;

  std::string to_string() {
    return std::format("{:%d%b%Y %H:%M:%S}",
                       std::chrono::floor<std::chrono::seconds>(tp));
  };

public:
  /** @brief Date as a displayable string. */
  std::string str;
  /** @brief Construct the date with the now() timepoint. */
  Date() {
    auto tp_now = std::chrono::system_clock::now();
    tp = std::chrono::floor<std::chrono::seconds>(tp_now);
    str = to_string();
  };
  /** @brief Construct the date from a UNIX timestamp in seconds. */
  Date(uint64_t unix_seconds) : tp (std::chrono::seconds{unix_seconds}),
                                str (to_string()){};
  /** @brief Get the date as a UNIX timestamp (UTC) in seconds. */
  uint64_t to_unix_timestamp () const {
    return tp.time_since_epoch().count();
  };
};

/** @brief Timesheet entry object. */
struct Entry {
  Id id;
  std::string project_name;
  std::string task_name;
  Date start;
  Date stop;
};

#endif // DATA_OBJECTS_H
