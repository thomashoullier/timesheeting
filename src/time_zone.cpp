#include "time_zone.h"
#include <chrono>

TimeZone &TimeZone::get(const std::string &tz_string) {
  static TimeZone instance {tz_string};
  return instance;
}

TimeZone::TimeZone(const std::string &tz_string)
    : zone(std::chrono::get_tzdb().locate_zone(tz_string)) {}
