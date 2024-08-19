#include "date.h"
#include <iomanip>
#include <sstream>
#include <format>

std::string Date::to_string () {
  return std::format("{:%d%b%Y %H:%M:%S}",
                     std::chrono::floor<std::chrono::seconds>(tp));
}

Date::Date() {
  auto tp_now = std::chrono::system_clock::now();
  tp = std::chrono::floor<std::chrono::seconds>(tp_now);
  str = to_string();
}

Date::Date(uint64_t unix_seconds)
    : tp(std::chrono::seconds{unix_seconds}), str(to_string()) {}

Date::Date(const std::string &date_str) {
  // TODO: std::chrono::parse is not implemented in gcc 13.3,
  //       but would be the best solution here.
  // date_str >> std::chrono::parse("{:%d%b%Y %H:%M:%S}", tp);
  std::tm tm{};
  std::stringstream ss{date_str};
  ss >> std::get_time(&tm, "%d%b%Y %H:%M:%S");
  if (ss.fail()) {
    throw DateParsingFailure("Failed to parse the inputted date string.");
    return;
  }
  auto tp_parsed = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  tp = std::chrono::floor<std::chrono::seconds>(tp_parsed);
  str = to_string();
}

uint64_t Date::to_unix_timestamp() const {
  return tp.time_since_epoch().count();
}
