#ifndef TIME_ZONE_H
#define TIME_ZONE_H

#include <chrono>
#include <string>

/** @brief Singleton for program-wide timezones. */
class TimeZone {
public:
  static TimeZone& get(const std::string &tz_string = "");

  /** @brief Program-wide zone set. */
  const std::chrono::time_zone *zone;

private:
  TimeZone(const std::string &tz_string);
};

#endif // TIME_ZONE_H
