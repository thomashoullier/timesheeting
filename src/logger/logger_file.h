/** @file
 * @brief Logger interface implementation with a log file. */
#ifndef LOGGER_FILE_H
#define LOGGER_FILE_H

#include "../logger_interface.h"
#include <filesystem>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <chrono>

/** @brief Logger implementation with a log file. */
class LoggerFile : public LoggerInterface {
private:
  /** @brief Handle to the log file for output. */
  std::unique_ptr<std::ofstream> file;
  /** @brief Get the current date as a timestamp. */
  std::string get_timestamp ();
public:
  /** @brief Logger constructor using a file. */
  LoggerFile (const std::filesystem::path &log_file_path);

  void log (const std::string &message) override;
};

#endif // LOGGER_FILE_H
