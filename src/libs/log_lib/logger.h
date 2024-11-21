/** @file
 * @brief Logger interface implementation with a log file. */
#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <memory>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

namespace log_lib {
  enum class LogLevel { info, debug, error };

  /* TODO: Put these methods inside LogLevel. */
  std::string log_level_string(LogLevel level);
  LogLevel log_level_from_string (const std::string &level_str);

  /** @brief Logger implementation with a log file.
   *
   * This is a singleton. */
  class Logger {
    /** @brief Struct defining the active log levels. */
    struct ActiveLevels {
      bool info = false;
      bool debug = false;
      bool error = false;
    };

  public:
    /** @brief Grab the log file singleton.

        Initialize on the first call.*/
    static Logger& get(const std::filesystem::path &log_filepath = "",
                       const std::vector<std::string> &levels_to_log = {});
    void log (const std::string &message, LogLevel level = LogLevel::info);
    void tick ();
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
    Logger(const std::filesystem::path &log_file_path,
           const std::vector<std::string> &levels_to_log);
    /** @brief Get the current date as a timestamp. */
    std::string get_timestamp ();
    /** @brief Is the provided loglevel active? */
    bool level_is_active (LogLevel level);
  };

  /** @brief Grab the Logger. */
  Logger& logger();
} // namespace log_lib
#endif // LOGGER_H
