#ifndef TIME_ZONE_H
#define TIME_ZONE_H

#include <chrono>
#include <string>

namespace time_lib {
  /** @brief Singleton for program-wide timezones. */
  class TimeZone {
  public:
    static TimeZone& get(const std::string &tz_string = "");

    /** @brief Get the current timezone name (TZ string). */
    std::string name() const;

    /** @brief Program-wide zone set. */
    const std::chrono::time_zone *zone;

  private:
    TimeZone(const std::string &tz_string);
  };
}
#endif // TIME_ZONE_H
