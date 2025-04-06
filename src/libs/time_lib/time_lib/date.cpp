#include "date.h"
#include <chrono>
#include <format>
#include <sstream>
#include <stdexcept>
#include <vector>
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
      std::string current_year = std::format("{:%Y}", current);
      std::string first_day_str = "01Jan" + current_year;
      tp = time_point_from_str(first_day_str);
    } break;
    default:
      throw std::logic_error("Unknown DatePoint.");
    }
  }

  Date::Date(uint64_t unix_seconds)
    : tp(std::chrono::seconds{unix_seconds}) {}

  Date::Date(Date::SecondsAgo, uint64_t seconds_ago) {
    auto tp_now = std::chrono::system_clock::now();
    tp = std::chrono::floor<std::chrono::seconds>
      (tp_now - std::chrono::seconds{seconds_ago});
  }

  std::chrono::zoned_seconds Date::time_point_from_str
  (const std::string &date_str,
   const std::string &day_month_year_str) {
    std::stringstream ss{date_str};
    std::chrono::local_seconds local_tp;
    const std::vector<std::string> match_strings{
        "%d%b%Y %H:%M:%S", "%d%b%Y %H:%M", "%d%b%Y %H", "%d%b%Y"};
    for (const auto &match_string : match_strings) {
      ss >> std::chrono::parse(match_string, local_tp);
      if (ss.good()) {
        std::chrono::zoned_seconds zoned_sec(TimeZone::get().zone, local_tp);
        return zoned_sec;
      }
      ss.str(date_str);
      ss.clear();
    }
    if (not day_month_year_str.empty()) {
      ss.str(day_month_year_str + " " + date_str);
      for (const auto &match_string : match_strings) {
        ss >> std::chrono::parse(match_string, local_tp);
        if (ss.good()) {
          std::chrono::zoned_seconds zoned_sec(TimeZone::get().zone, local_tp);
          return zoned_sec;
        }
        ss.str(day_month_year_str + " " + date_str);
        ss.clear();
      }
    }
    throw DateParsingFailure("Failed to parse the inputted date string.");
  }

  Date::Date(const std::string &date_str,
             const std::string &day_month_year_str)
    : tp{time_point_from_str(date_str, day_month_year_str)} {}

  Date::Date(FullString, const std::string &date_fullstr) {
    std::stringstream ss{date_fullstr};
    ss >> std::chrono::parse("%d%b%Y %H:%M:%S %z", tp);
    if (not ss.good()) {
      throw DateParsingFailure("Failed to parse the inputted date string.");
    }
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
