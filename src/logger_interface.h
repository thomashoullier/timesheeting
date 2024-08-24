/** @file
 * @brief Logger virtual interface definition. */
#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H

#include <string>

/** @brief Virtual interface class for loggers. */
class LoggerInterface {
public:
  virtual ~LoggerInterface () = default;

  /** @brief Grab the single instance for the logger. */
  static LoggerInterface& get();
  /** @brief Log a message. */
  virtual void log (const std::string &msg) = 0;
  /** @brief Start a clock for timing a duration. */
  virtual void tick () = 0;
  /** @brief Stop the clock and log the duration since the first tick. */
  virtual void tock () = 0;
};

#endif // LOGGER_INTERFACE_H
