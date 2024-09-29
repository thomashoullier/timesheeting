/** @file
 * @brief Logger interface implementation with a log file. */
#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <memory>
#include <fstream>
#include <chrono>

enum class LogLevel { info, debug, error };

std::string log_level_string (LogLevel level);

/** @brief Logger implementation with a log file.
 *
 * This is a singleton. */
class Logger {
public:
  /** @brief Grab the log file singleton.

   Initialize on the first call.*/
  static Logger& get(const std::filesystem::path &log_filepath = "");
  void log (const std::string &message, LogLevel level = LogLevel::info);
  void tick ();
  void tock ();

private:
  /** @brief Handle to the log file for output. */
  std::unique_ptr<std::ofstream> file;
  /** @brief Start time point for clocking durations. */
  std::chrono::high_resolution_clock::time_point tick_point;
  /** @brief Clock state. */
  bool counting;

  /** @brief Logger constructor using a file. */
  Logger (const std::filesystem::path &log_file_path);
  /** @brief Get the current date as a timestamp. */
  std::string get_timestamp ();
};

/** @brief Grab the Logger. */
Logger& logger();

#endif // LOGGER_H
