#include "logger.h"
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ratio>
#include <sstream>
#include <stdexcept>
#include <string>
#include "time_lib/date.h"
#include "temp_dir.h"

namespace log_lib{
  Logger &Logger::get(const std::filesystem::path &log_filepath,
                      const std::vector<std::string> &levels_to_log,
                      int64_t max_log_age) {
    static Logger instance (log_filepath, levels_to_log, max_log_age);
    return instance;
  }

  void Logger::log(const std::string &message, LogLevel level) {
    if (message.find('\n') < message.length()) {
      throw std::runtime_error("Log message must fit on a single line.");
    }
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

  void Logger::tock(const std::string &message) {
    auto tock_point = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = tock_point - tick_point;
    counting = false;
    std::ostringstream ostr;
    ostr.precision(3);
    ostr << std::fixed << dur.count();
    const std::string log_str = message + ostr.str() + " ms.";
    this->log(log_str, LogLevel::debug);
  }

  Logger::Logger(const std::filesystem::path &log_file,
                 const std::vector<std::string> &levels_to_log,
                 int64_t max_log_age)
    : counting(false) {
    // Cleaning up the log file first.
    remove_old_entries(log_file, max_log_age);
    file = std::make_unique<std::ofstream>(log_file,
                                           std::ios::app);
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
    time_lib::Date now;
    return now.to_fullstring();
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

  void Logger::remove_old_entries (const std::filesystem::path &log_file_path,
                                   int64_t seconds_ago) {
    if (not(std::filesystem::exists(log_file_path))
        or std::filesystem::is_empty(log_file_path))
      return;
    std::ifstream old_file(log_file_path);
    if (!old_file.is_open())
      throw std::runtime_error("Failed to open old log file");
    auto oldest_date =
        time_lib::Date{time_lib::Date::SecondsAgo{}, seconds_ago};
    std::string line;
    auto temp_dir = TempDir{"timesheeting_log_rotation"};
    auto new_file_path = temp_dir.dirpath / "timesheeting.log";
    auto new_file = std::ofstream(new_file_path,
                                  std::ios::app);
    if (!new_file.good()) {
      throw std::runtime_error("New log file was not opened correctly.");
    }
    while (std::getline(old_file, line)) {
      // The date part of the log entry string has fixed length of 24
      auto date_str = line.substr(0, 24);
      auto date = time_lib::Date{time_lib::Date::FullString{}, date_str};
      if (date > oldest_date) {
        new_file << line << std::endl;
      }
    }
    old_file.close();
    new_file.close();
    std::filesystem::rename(new_file_path, log_file_path);
  }

  std::string Logger::log_level_string(LogLevel level) {
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

  LogLevel Logger::log_level_from_string(const std::string &level_str) {
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

  Logger &logger() { return Logger::get(); }
}
