#include "logger.h"
#include <chrono>
#include <ratio>
#include <sstream>
#include <stdexcept>
#include <string>

std::string log_level_string (LogLevel level) {
  switch (level) {
  case LogLevel::info:
    return "info";
    break;
  case LogLevel::debug:
    return "debug";
    break;
  case LogLevel::error:
    return "error";
    break;
  default:
    throw std::runtime_error("Unknown LogLevel");
  }
}

Logger &Logger::get(const std::filesystem::path &log_filepath) {
  static Logger instance (log_filepath);
  return instance;
}

void Logger::log(const std::string &message, LogLevel level) {
  if (!file->good()) {
    throw std::runtime_error("Log file cannot be written to.");
  }
  *file << get_timestamp() << " "
        << "[" << log_level_string(level) << "] "
        << ": " << message << std::endl;
}

void Logger::tick() {
  if (counting) {
    return;
  } else {
    counting = true;
    tick_point = std::chrono::high_resolution_clock::now();
  }
}

void Logger::tock() {
  auto tock_point = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> dur = tock_point - tick_point;
  std::ostringstream ostr;
  ostr.precision(3);
  ostr << std::fixed << dur.count();
  this->log("Input loop was: " + ostr.str() + " ms.", LogLevel::debug);
  counting = false;
}

Logger::Logger(const std::filesystem::path &log_file)
    : file(std::make_unique<std::ofstream>(log_file, std::ios::app)),
      counting(false) {
  if (!file->good()) {
    throw std::runtime_error("Log file was not opened correctly.");
  }
}

std::string Logger::get_timestamp() {
  auto date = std::chrono::system_clock::now();
  auto date_t = std::chrono::system_clock::to_time_t(date);
  char timeString[std::size("yyyy-mm-dd hh:mm:ss")];
  std::strftime(std::data(timeString), std::size(timeString),
                "%F %T", std::localtime(&date_t));
  return timeString;
}

Logger& logger() { return Logger::get(); }
