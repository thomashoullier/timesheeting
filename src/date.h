#ifndef DATE_H
#define DATE_H

#include <cstdint>
#include <chrono>
#include <ctime>
#include <exception>
#include <string>

/** @brief Utility to designate date points. */
enum class DatePoint { DayBegin, DayEnd };

/** @brief Date type. */
class Date {
private:
  /** @brief Internal time point representation, seconds UTC */
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp;

  /** @brief Convert the internal timepoint to a displayable date string
      in the system's timezone.

      Note the system's timezone is defined by `/etc/localtime` here,
      not the TZ environment variable. */
  std::string to_string();

public:
  /** @brief Date as a displayable string. */
  std::string str;

  /** @brief Construct the date with the now() timepoint. */
  Date();
  /** @brief Construct the date at the given date point. */
  Date(DatePoint date_point);
  /** @brief Construct the date from a UNIX timestamp in seconds. */
  Date(uint64_t unix_seconds);
  /** @brief Construct the date from a string in a fixed format:
      '%d%b%Y %H:%M:%S'.
      This is the same format as the display string format.*/
  Date(const std::string &date_str);

  /** @brief Get the date as a UNIX timestamp (UTC) in seconds. */
  uint64_t to_unix_timestamp () const;
  /** @brief Get a displayable string for the date at a day resolution. */
  std::string get_day_string () const;
  /** @brief Increment the date by one day. */
  void add_one_day ();
  /** @brief Decrement the date by one day. */
  void subtract_one_day ();
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
