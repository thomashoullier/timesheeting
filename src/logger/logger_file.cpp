#include "logger_file.h"

LoggerFile& LoggerFile::get() {
  static LoggerFile instance ("timesheeting.log");
  return instance;
}

void LoggerFile::log(const std::string &message) {
  if (!file->good()) {
    throw std::runtime_error("Log file cannot be written to.");
  }
  *file << get_timestamp() << " : " << message << std::endl;
}

LoggerFile::LoggerFile(const std::filesystem::path &log_file)
    : file(std::make_unique<std::ofstream>(log_file, std::ios::app)) {
  if (!file->good()) {
    throw std::runtime_error("Log file was not opened correctly.");
  }
}

std::string LoggerFile::get_timestamp() {
  auto date = std::chrono::system_clock::now();
  auto date_t = std::chrono::system_clock::to_time_t(date);
  char timeString[std::size("yyyy-mm-dd hh:mm:ss")];
  std::strftime(std::data(timeString), std::size(timeString),
                "%F %T", std::localtime(&date_t));
  return timeString;
}
