#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <filesystem>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <chrono>

class Logger {
private:
  std::unique_ptr<std::ofstream> file; // Log file handle
  std::string get_timestamp ();
public:
  Logger (const std::filesystem::path &log_file_path);

  void log (const std::string &message);
};

#endif // LOGGER_H
