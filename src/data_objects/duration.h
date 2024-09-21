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
  std::string to_string() const;
  /** @brief Displayable short string of the duration in hours,
   omitting the units and putting blanks over zeros. */
  std::string to_shortstring() const;
};

#endif // DURATION_H
