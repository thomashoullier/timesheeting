/** @file
 * @brief Date object definition. */
#ifndef DATE_H
#define DATE_H

#include <cstdint>
#include <chrono>
#include <ctime>
#include <exception>
#include <string>

namespace time_lib{
  /** @brief Shorthands for common date points. */
  enum class DatePoint { YearBegin };

  /** @brief Date object.

      The time is internally represented in UTC, and inputted/displayed in the
      current system's timezone. */
  class Date {
  private:
    /** @brief Compute a time_point from a date string. */
    std::chrono::zoned_seconds time_point_from_str (const std::string &date_str);

  public:
    /** @brief Internal time point representation, seconds UTC */
    std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp;

    /** @brief Construct the date with the now() timepoint. */
    explicit Date();
    /** @brief Construct the date from a given timepoint. */
    explicit Date(std::chrono::time_point<std::chrono::system_clock,
                  std::chrono::seconds> _tp);
    /** @brief Construct the date at the given date point shorthand. */
    explicit Date(DatePoint date_point);
    /** @brief Construct the date from a UNIX timestamp (UTC) in seconds. */
    explicit Date(uint64_t unix_seconds);
    /** @brief Tag for creating a Date from uint64_t. */
    struct SecondsAgo final {};
    /** @brief Create a Date at the given amount of seconds ago from now
               in the past. */
    explicit Date(SecondsAgo, uint64_t seconds_ago);
    /** @brief Construct the date from a string in fixed format.

        "%d%b%Y %H:%M:%S" is tried first, then successively smaller
        matches are tried until "%d%b%Y". Omitted information is
        replaced with zeroes. */
    explicit Date(const std::string &date_str);
    /** @brief Tag for creating a Date from a string. */
    struct FullString final {};
    /** @brief Parse a Date from a fullstring format. */
    explicit Date(FullString, const std::string &date_fullstr);

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
    /** @brief Convert the internal timepoint into a full unambiguous date string
        with timezone information. */
    std::string to_fullstring() const;
    /** @brief Get the date as a UNIX timestamp (UTC) in seconds. */
    uint64_t to_unix_timestamp () const;
    /** @brief Get a displayable string for the date at a day resolution
        in the system's timezone. */
    std::string get_day_string () const;
    /** @brief Date less than operator. */
    friend bool operator< (Date date_1, Date date_2);
    /** @brief Date more than operator. */
    friend bool operator> (Date date_1, Date date_2);
  };

  /** @brief Exception indicating that Date has failed to parse
      an input string. */
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
} // namespace time_lib
#endif // DATE_H
