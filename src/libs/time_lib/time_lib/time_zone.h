/** @file
 *  @brief TimeZone definition. */
#ifndef TIME_ZONE_H
#define TIME_ZONE_H

#include <chrono>
#include <string>

namespace time_lib {
  /** @brief Program-wide timezone setting.

   Singleton pattern. */
  class TimeZone {
  public:
    /** @brief Grab the singleton. Initialize on first call. */
    static TimeZone& get(const std::string &tz_string = "");

    /** @brief Get the current timezone name (TZ string). */
    std::string name() const;

    /** @brief Program-wide zone set. */
    const std::chrono::time_zone *zone;

    TimeZone(TimeZone const&) = delete;
    void operator=(TimeZone const&) = delete;

  private:
    /** @brief Singleton constructor. */
    explicit TimeZone(const std::string &tz_string);
  };
}
#endif // TIME_ZONE_H
