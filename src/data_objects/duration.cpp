#include "duration.h"
#include <sstream>

Duration::Duration() : dur(std::chrono::seconds(0)) {}

Duration::Duration(uint64_t seconds) : dur(std::chrono::seconds(seconds)) {}

std::string Duration::to_string() const {
  double hours = static_cast<double>(dur.count()) / 3600;
  std::ostringstream ostr;
  ostr << std::fixed << std::setprecision(3) << hours << " hours";
  return ostr.str();
}

std::string Duration::to_shortstring() const {
  if (dur.count() == 0) {
    return " ";
  }
  double hours = static_cast<double>(dur.count()) / 3600;
  std::ostringstream ostr;
  ostr << std::fixed << std::setprecision(3) << hours;
  return ostr.str();
}
