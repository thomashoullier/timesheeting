#ifndef LOGGER_FILE_H
#define LOGGER_FILE_H

#include "../logger_interface.h"
#include <filesystem>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <chrono>

class LoggerFile : public LoggerInterface {
private:
  std::unique_ptr<std::ofstream> file; // Log file handle
  std::string get_timestamp ();
public:
  LoggerFile (const std::filesystem::path &log_file_path);

  void log (const std::string &message) override;
};

#endif // LOGGER_FILE_H
