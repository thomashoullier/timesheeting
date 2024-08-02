#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H

#include <string>

class LoggerInterface {
public:
  virtual ~LoggerInterface () = default;

  /** Log a message. */
  virtual void log (const std::string &msg) = 0;
};

#endif // LOGGER_INTERFACE_H
