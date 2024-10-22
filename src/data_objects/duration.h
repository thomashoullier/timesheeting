#ifndef DURATION_H
#define DURATION_H

#include <cstdint>
#include <chrono>

/** @brief Duration type*/
class Duration {
private:
  /** @brief Duration represented as a number of seconds. */
  std::chrono::seconds dur;

public:
  /** @brief Construct a duration of 0 by default. */
  Duration();
  /** @brief Initialize the duration to a number of seconds. */
  Duration (uint64_t seconds);
  /** @brief Obtain a displayable representation for the duration in
      hours with 3 decimal places. */
  std::string to_hour_string() const;
  /** @brief Displayable short string of the duration in hours,
   omitting the units and putting blanks over zeros. */
  std::string to_hour_shortstring() const;
  /** @brief Obtain a displayable representation for the duration in seconds. */
  std::string to_second_string() const;
  /** @brief Displayable short string of the duration in seconds. */
  std::string to_second_shortstring() const;
  /** @brief Obtain a displayable representation for the duration in days,
      provided a number of hours (typically of work) per day. */
  std::string to_day_string(float hours_per_day) const;
  /** @brief Displayable short string of the duration in days. */
  std::string to_day_shortstring(float hours_per_day) const;
};

#endif // DURATION_H
