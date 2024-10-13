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

LogLevel log_level_from_string (const std::string &level_str) {
  if (level_str == "info") {
    return LogLevel::info;
  } else if (level_str == "debug") {
    return LogLevel::debug;
  } else if (level_str == "error") {
    return LogLevel::error;
  } else {
    throw std::runtime_error("log_level_from_string: unknown log level.");
  }
}

Logger &Logger::get(const std::filesystem::path &log_filepath,
                    const std::vector<std::string> &levels_to_log) {
  static Logger instance (log_filepath, levels_to_log);
  return instance;
}

void Logger::log(const std::string &message, LogLevel level) {
  if (level_is_active(level)) {
    if (!file->good()) {
      throw std::runtime_error("Log file cannot be written to.");
    }
    *file << get_timestamp() << " "
          << "[" << log_level_string(level) << "] "
          << ": " << message << std::endl;
  }
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

Logger::Logger(const std::filesystem::path &log_file,
               const std::vector<std::string> &levels_to_log)
    : file(std::make_unique<std::ofstream>(log_file, std::ios::app)),
      counting(false) {
  if (!file->good()) {
    throw std::runtime_error("Log file was not opened correctly.");
  }
  // Initialize the active log levels.
  for (const auto &level_str : levels_to_log) {
    auto level = log_level_from_string(level_str);
    switch (level) {
    case LogLevel::info:
      active_levels.info = true;
      break;
    case LogLevel::debug:
      active_levels.debug = true;
      break;
    case LogLevel::error:
      active_levels.error = true;
      break;
    default:
      throw std::runtime_error("Unknown LogLevel");
    }
  }
}

std::string Logger::get_timestamp() {
  auto date = std::chrono::system_clock::now();
  auto date_t = std::chrono::system_clock::to_time_t(date);
  char timeString[std::size("yyyy-mm-dd hh:mm:ss")];
  std::strftime(std::data(timeString), std::size(timeString), "%F %T",
                std::localtime(&date_t));
  return timeString;
}

bool Logger::level_is_active(LogLevel level) {
  switch (level) {
  case LogLevel::info:
    return active_levels.info;
  case LogLevel::debug:
    return active_levels.debug;
  case LogLevel::error:
    return active_levels.error;
  default:
    throw std::runtime_error("Unknown LogLevel");
  }
}

Logger& logger() { return Logger::get(); }
