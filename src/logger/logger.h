/** @file
 * @brief Logger interface implementation with a log file. */
#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <memory>
#include <fstream>
#include <chrono>

/** @brief Logger implementation with a log file.
 *
 * This is a singleton. */
class Logger {
public:
  /** @brief Grab the log file singleton. */
  static Logger& get();
  void log (const std::string &message);
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

/** @brief Grab the logger and call log(). */
void log(const std::string &msg);
/** @brief Grab the logger and call tick(). */
void tick();
/** @brief Grab the logger and call tock(). */
void tock();

#endif // LOGGER_H
