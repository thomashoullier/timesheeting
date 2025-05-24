#include "date_range.h"
#include <stdexcept>

namespace time_lib {
  DateRange::DateRange(const Date &_start, const Date &_stop)
    : start(_start), stop(_stop) {
    if (_start > _stop) {
      throw std::runtime_error("DateRange initialization: start Date is greater "
                               "than stop Date.");
    }
  }

  std::vector<std::string> DateRange::to_string() const {
    std::vector<std::string> strings(2);
    strings.at(0) = start.to_string();
    strings.at(1) = stop.to_string();
    return strings;
  }

  std::vector<std::string> DateRange::to_day_strings() const {
    std::vector<std::string> strings(2);
    strings.at(0) = start.get_day_string();
    strings.at(1) = stop.get_day_string();
    return strings;
  }

  bool DateRange::contains(const Date &date) const {
    return date >= start and date <= stop;
  }
} // namespace time_lib
