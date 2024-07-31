#ifndef STATUS_BAR_INTERFACE_H
#define STATUS_BAR_INTERFACE_H

#include <string>

class StatusBarInterface {
public:
  virtual ~StatusBarInterface () = default;

  /** Print the message in the status bar. */
  virtual void print (const std::string &msg) = 0;
  /** Print the message in the status bar, and then pause until user input. */
  virtual void print_wait (const std::string &msg) = 0;
  /** Ask for confirmation. */
  virtual bool query_confirmation (const std::string &msg) = 0;
};

#endif // STATUS_BAR_INTERFACE_H
