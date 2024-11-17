#include "date.h"
#include <bits/chrono.h>
#include <chrono>
#include <format>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "time_zone.h"

namespace time_lib{
  Date::Date() {
    auto tp_now = std::chrono::system_clock::now();
    tp = std::chrono::floor<std::chrono::seconds>(tp_now);
  }

  Date::Date(std::chrono::time_point<std::chrono::system_clock,
             std::chrono::seconds> _tp)
    : tp{_tp} {}

  Date::Date(DatePoint date_point) {
    switch(date_point) {
    case DatePoint::YearBegin: {
      std::chrono::zoned_time current{TimeZone::get().zone,
                                      std::chrono::system_clock::now()};
      // TODO: this is hacky, is there no better way?
      std::chrono::zoned_time first_day
        {TimeZone::get().zone,
         std::chrono::floor<std::chrono::days>
         (std::chrono::floor<std::chrono::years>(current.get_local_time()))};
      tp = first_day.get_sys_time();
    } break;
    default:
      throw std::logic_error("Unknown date_point.");
    }
  }

  Date::Date(uint64_t unix_seconds)
    : tp(std::chrono::seconds{unix_seconds}) {}

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
    // TODO: this is a hack, works only for our current timezone.
    tm.tm_isdst = true;
    auto tp_parsed = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    tp = std::chrono::floor<std::chrono::seconds>(tp_parsed);
  }

  std::string Date::to_string() const {
    std::chrono::zoned_seconds local_time{TimeZone::get().zone, tp};
    return std::format("{:%d%b%Y %H:%M:%S}", local_time);
  }

  std::string Date::to_shortstring() const {
    std::chrono::zoned_seconds local_time{TimeZone::get().zone, tp};
    return std::format("{:%H:%M}", local_time);
  }

  std::string Date::to_fullstring() const {
    std::chrono::zoned_seconds local_time{TimeZone::get().zone, tp};
    return std::format("{:%d%b%Y %H:%M:%S %z}", local_time);
  }

  uint64_t Date::to_unix_timestamp() const {
    return tp.time_since_epoch().count();
  }

  std::string Date::get_day_string () const {
    std::chrono::zoned_seconds local_time {TimeZone::get().zone, tp};
    return std::format("{:%d%b%Y}", local_time);
  }

  bool operator< (Date date_1, Date date_2) {
    return date_1.to_unix_timestamp() < date_2.to_unix_timestamp();
  }

  bool operator>(Date date_1, Date date_2) {
    return date_1.to_unix_timestamp() > date_2.to_unix_timestamp();
  }
}
