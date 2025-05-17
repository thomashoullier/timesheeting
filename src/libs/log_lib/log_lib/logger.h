/** @file
 * @brief Logger interface implementation with a log file. */
#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

namespace log_lib {
  /** @brief Categorization for log messages. */
  enum class LogLevel { info, debug, error };

  /** @brief Logger implementation with a log file.
   *
   * This is a singleton. */
  class Logger {
    /** @brief Struct defining the active log levels. */
    struct ActiveLevels {
      /** @brief Is the 'info' log level active? */
      bool info = false;
      /** @brief Is the 'debug' log level active? */
      bool debug = false;
      /** @brief Is the 'error' log level active? */
      bool error = false;
    };

  public:
    /** @brief Grab the log file singleton.

        Initialize on the first call.*/
    static Logger& get(const std::filesystem::path &log_filepath = "",
                       const std::vector<std::string> &levels_to_log = {},
                       uint64_t max_log_age = 0);
    /** @brief Log a message at the given level. */
    void log (const std::string &message, LogLevel level = LogLevel::info);
    /** @brief Start a time counter. */
    void tick ();
    /** @brief Stop the time counter and log the elapsed duration. */
    void tock ();

  private:
    /** @brief Handle to the log file for output. */
    std::unique_ptr<std::ofstream> file;
    /** @brief Active log levels. */
    ActiveLevels active_levels;
    /** @brief Start time point for clocking durations. */
    std::chrono::high_resolution_clock::time_point tick_point;
    /** @brief Clock state. */
    bool counting;

    /** @brief Logger constructor using a file. */
    explicit Logger(const std::filesystem::path &log_file_path,
                    const std::vector<std::string> &levels_to_log,
                    uint64_t max_log_age);
    /** @brief Get the current date as a timestamp. */
    std::string get_timestamp ();
    /** @brief Is the provided loglevel active? */
    bool level_is_active (LogLevel level);
    /** @brief Log cleanup. Remove entries older than the specified age. */
    void remove_old_entries (const std::filesystem::path &log_file_path,
                             uint64_t seconds_ago);

    /** @brief Convert a LogLevel to its string representation. */
    std::string log_level_string(LogLevel level);
    /** @brief Get a log level from a string representation. */
    LogLevel log_level_from_string (const std::string &level_str);
  };

  /** @brief Grab the Logger. */
  Logger& logger();
} // namespace log_lib
#endif // LOGGER_H
