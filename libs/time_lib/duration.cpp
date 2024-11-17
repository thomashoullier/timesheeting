#include "duration.h"
#include <sstream>
#include <stdexcept>

namespace time_lib {
  Duration::Duration() : dur(std::chrono::seconds(0)) {}

  Duration::Duration(uint64_t seconds) : dur(std::chrono::seconds(seconds)) {}

  std::string Duration::to_hour_string() const {
    double hours = static_cast<double>(dur.count()) / 3600;
    std::ostringstream ostr;
    ostr << std::fixed << std::setprecision(3) << hours << " hours";
    return ostr.str();
  }

  std::string Duration::to_hour_shortstring() const {
    if (dur.count() == 0)
      return " ";
    double hours = static_cast<double>(dur.count()) / 3600;
    std::ostringstream ostr;
    ostr << std::fixed << std::setprecision(3) << hours;
    return ostr.str();
  }

  std::string Duration::to_second_string() const {
    auto seconds = dur.count();
    std::ostringstream ostr;
    ostr << seconds << " seconds";
    return ostr.str();
  }

  std::string Duration::to_second_shortstring() const {
    if (dur.count() == 0)
      return " ";
    auto seconds = dur.count();
    std::ostringstream ostr;
    ostr << seconds;
    return ostr.str();
  }

  std::string Duration::to_day_string(float hours_per_day) const {
    if (hours_per_day <= 0.0)
      throw std::runtime_error("hours_per_day is non-positive.");
    float days = static_cast<float>(dur.count()) / 3600.0 / hours_per_day;
    std::ostringstream ostr;
    ostr << std::fixed << std::setprecision(3) << days << " days";
    return ostr.str();
  }

  std::string Duration::to_day_shortstring(float hours_per_day) const {
    if (hours_per_day <= 0.0)
      throw std::runtime_error("hours_per_day is non-positive.");
    if (dur.count() == 0)
      return " ";
    float days = static_cast<float>(dur.count()) / 3600.0 / hours_per_day;
    std::ostringstream ostr;
    ostr << std::fixed << std::setprecision(3) << days;
    return ostr.str();
  }
}
