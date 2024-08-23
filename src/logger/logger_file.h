/** @file
 * @brief Logger interface implementation with a log file. */
#ifndef LOGGER_FILE_H
#define LOGGER_FILE_H

#include "../logger_interface.h"
#include <filesystem>
#include <memory>
#include <fstream>

/** @brief Logger implementation with a log file.
 *
 * This is a singleton. */
class LoggerFile : public LoggerInterface {
public:
  /** @brief Grab the log file singleton. */
  static LoggerFile& get();
  void log (const std::string &message) override;

private:
  /** @brief Logger constructor using a file. */
  LoggerFile (const std::filesystem::path &log_file_path);
  /** @brief Handle to the log file for output. */
  std::unique_ptr<std::ofstream> file;
  /** @brief Get the current date as a timestamp. */
  std::string get_timestamp ();
};

#endif // LOGGER_FILE_H
