#ifndef DATE_H
#define DATE_H

#include <cstdint>
#include <chrono>
#include <ctime>
#include <exception>
#include <string>

/** @brief Date type. */
class Date {
private:
  /** @brief Internal time point representation. */
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp;

  /** @brief Convert the internal timepoint to a displayable date string. */
  std::string to_string();

public:
  /** @brief Date as a displayable string. */
  std::string str;

  /** @brief Construct the date with the now() timepoint. */
  Date();
  /** @brief Construct the date from a UNIX timestamp in seconds. */
  Date(uint64_t unix_seconds);
  /** @brief Construct the date from a string in a fixed format:
      '%d%b%Y %H:%M:%S'.
      This is the same format as the display string format.*/
  Date(const std::string &date_str);

  /** @brief Get the date as a UNIX timestamp (UTC) in seconds. */
  uint64_t to_unix_timestamp () const;
};

class DateParsingFailure : public std::exception {
private:
  std::string msg;

public:
  DateParsingFailure (const char* _msg) : msg(_msg) {};
  const char* what() const throw() {
    return msg.c_str();
  };
};

#endif // DATE_H
