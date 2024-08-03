/** @file
 * @brief Logger virtual interface definition. */
#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H

#include <string>

/** @brief Virtual interface class for loggers. */
class LoggerInterface {
public:
  virtual ~LoggerInterface () = default;

  /** @brief Log a message. */
  virtual void log (const std::string &msg) = 0;
};

#endif // LOGGER_INTERFACE_H
