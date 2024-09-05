#include "date.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <format>
#include <stdexcept>

Date::Date() {
  auto tp_now = std::chrono::system_clock::now();
  tp = std::chrono::floor<std::chrono::seconds>(tp_now);
}

Date::Date(DatePoint date_point) {
  switch(date_point) {
  case DatePoint::DayBegin: {
    std::chrono::zoned_time current{std::chrono::current_zone(),
                                    std::chrono::system_clock::now()};
    std::chrono::zoned_time midnight
      {std::chrono::current_zone(),
       std::chrono::floor<std::chrono::days>(current.get_local_time())};
    tp = midnight.get_sys_time();
  } break;
  case DatePoint::DayEnd: {
    std::chrono::zoned_time current{std::chrono::current_zone(),
                                    std::chrono::system_clock::now()};
    std::chrono::zoned_time midnight
      {std::chrono::current_zone(),
       std::chrono::ceil<std::chrono::days>(current.get_local_time())};
    tp = midnight.get_sys_time();
  } break;
  case DatePoint::WeekBegin: {
    std::chrono::zoned_time current{std::chrono::current_zone(),
                                    std::chrono::system_clock::now()};
    auto beg_day = std::chrono::floor<std::chrono::days>
      (current.get_local_time());
    std::chrono::weekday day = std::chrono::weekday {beg_day};
    auto gap = day - std::chrono::Monday;
    auto monday = beg_day - gap;
    std::chrono::zoned_time monday_start {std::chrono::current_zone(),
                                          monday};
    tp = monday_start;
  } break;
  case DatePoint::YearBegin: {
    std::chrono::zoned_time current{std::chrono::current_zone(),
                                    std::chrono::system_clock::now()};
    // TODO: this is hacky, is there no better way?
    std::chrono::zoned_time first_day
      {std::chrono::current_zone(),
       std::chrono::floor<std::chrono::days>
       (std::chrono::floor<std::chrono::years>(current.get_local_time()))};
    tp = first_day.get_sys_time();
  } break;
  default:
    throw std::logic_error("Unknown date_point.");
  }
}

Date::Date(uint64_t unix_seconds)
    : tp(std::chrono::seconds{unix_seconds}) {}

Date::Date(const std::string &date_str) {
  // TODO: std::chrono::parse is not implemented in gcc 13.3,
  //       but would be the best solution here.
  // date_str >> std::chrono::parse("{:%d%b%Y %H:%M:%S}", tp);
  std::tm tm{};
  std::stringstream ss{date_str};
  ss >> std::get_time(&tm, "%d%b%Y %H:%M:%S");
  if (ss.fail()) {
    throw DateParsingFailure("Failed to parse the inputted date string.");
    return;
  }
  tm.tm_isdst = true; // TODO: this is a hack, works only for our current timezone.
  auto tp_parsed = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  tp = std::chrono::floor<std::chrono::seconds>(tp_parsed);
}

std::string Date::to_string () const {
  std::chrono::zoned_seconds local_time {std::chrono::current_zone(), tp};
  return std::format("{:%d%b%Y %H:%M:%S}", local_time);
}

std::string Date::to_shortstring() const {
  std::chrono::zoned_seconds local_time {std::chrono::current_zone(), tp};
  return std::format("{:%H:%M}", local_time);
}

uint64_t Date::to_unix_timestamp() const {
  return tp.time_since_epoch().count();
}

std::string Date::get_day_string () const {
  std::chrono::zoned_seconds local_time {std::chrono::current_zone(), tp};
  return std::format("{:%d%b%Y}", local_time);
}

void Date::add_one_day() {
    // TODO isn't there a way to add one day without having to round
    // to midnight?
    std::chrono::zoned_time next_day{std::chrono::current_zone(),
                                     tp + std::chrono::days(1)};
    std::chrono::zoned_time midnight
      {std::chrono::current_zone(),
       std::chrono::round<std::chrono::days>(next_day.get_local_time())};
    tp = midnight.get_sys_time();
}

void Date::subtract_one_day() {
  std::chrono::zoned_time prev_day{std::chrono::current_zone(),
                                   tp - std::chrono::days(1)};
  std::chrono::zoned_time midnight{
      std::chrono::current_zone(),
      std::chrono::round<std::chrono::days>(prev_day.get_local_time())};
  tp = midnight.get_sys_time();
}
