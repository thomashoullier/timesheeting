/** @file
 * @brief Date object definition. */
#ifndef DATE_H
#define DATE_H

#include <cstdint>
#include <chrono>
#include <ctime>
#include <exception>
#include <string>

/** @brief Shorthands for common date points. */
enum class DatePoint { DayBegin, DayEnd, YearBegin, WeekBegin };

/** @brief Date object.

 The time is internally represented in UTC, and inputted/displayed in the
current system's timezone. */
class Date {
private:
  /** @brief Internal time point representation, seconds UTC */
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp;

public:
  /** @brief Construct the date with the now() timepoint. */
  Date();
  /** @brief Construct the date at the given date point shorthand. */
  Date(DatePoint date_point);
  /** @brief Construct the date from a UNIX timestamp (UTC) in seconds. */
  Date(uint64_t unix_seconds);
  /** @brief Construct the date from a string in a fixed format:
      "%d%b%Y %H:%M:%S".
      This is the same format as the display string format.*/
  Date(const std::string &date_str);

  /** @brief Convert the internal timepoint to a displayable date string
      in the system's timezone.

      Note the system's timezone is defined by `/etc/localtime` here,
      not the TZ environment variable. */
  std::string to_string() const;
  /** @brief Convert the internal timepoint to a short displayable date string
      in the system's timezone.

      Note the system's timezone is defined by `/etc/localtime` here,
      not the TZ environment variable. */
  std::string to_shortstring() const;
  /** @brief Get the date as a UNIX timestamp (UTC) in seconds. */
  uint64_t to_unix_timestamp () const;
  /** @brief Get a displayable string for the date at a day resolution
      in the system's timezone. */
  std::string get_day_string () const;
  /** @brief Increment the date by one day
      and round to nearest local midnight. */
  void add_one_day ();
  /** @brief Decrement the date by one day
      and round to nearest local midnight. */
  void subtract_one_day ();
  /** @brief Increment the date by one week,
      and round to nearest local midnight. */
  void add_one_week();
};

/** @brief Exception indicating that Date has failed to parse an input string. */
class DateParsingFailure : public std::exception {
private:
  /** @brief Exception message. */
  std::string msg;

public:
  /** @brief Exception constructor. */
  DateParsingFailure (const char* _msg) : msg(_msg) {};
  /** @brief Mandatory std::exception method. */
  const char* what() const throw() {
    return msg.c_str();
  };
};

#endif // DATE_H
