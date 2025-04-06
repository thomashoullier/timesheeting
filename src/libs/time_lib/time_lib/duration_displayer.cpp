#include "duration_displayer.h"
#include "duration.h"
#include <stdexcept>

namespace time_lib {
  DurationDisplayer &DurationDisplayer::get(float hours_per_day) {
    static DurationDisplayer instance {hours_per_day};
    return instance;
  }

  void DurationDisplayer::cycle_format() {
    switch (current_format) {
    case DurationFormat::hours:
      current_format = DurationFormat::days;
      break;
    case DurationFormat::days:
      current_format = DurationFormat::seconds;
      break;
    case DurationFormat::seconds:
      current_format = DurationFormat::hours;
      break;
    default:
      throw std::runtime_error("Unknown DurationFormat.");
    }
  }

  std::string DurationDisplayer::to_string(const time_lib::Duration &dur) {
    switch (current_format) {
    case DurationFormat::hours:
      return dur.to_hour_string();
      break;
    case DurationFormat::days:
      return dur.to_day_string(hours_per_day);
      break;
    case DurationFormat::seconds:
      return dur.to_second_string();
      break;
    default:
      throw std::runtime_error("Unknown DurationFormat.");
    }
  }

  std::string DurationDisplayer::to_shortstring(const time_lib::Duration &dur) {
    switch (current_format) {
    case DurationFormat::hours:
      return dur.to_hour_shortstring();
      break;
    case DurationFormat::days:
      return dur.to_day_shortstring(hours_per_day);
      break;
    case DurationFormat::seconds:
      return dur.to_second_shortstring();
      break;
    default:
      throw std::runtime_error("Unknown DurationFormat.");
    }
  }

  DurationDisplayer::DurationDisplayer(float hours_per_day)
    : current_format(DurationFormat::hours),
      hours_per_day{hours_per_day} {}
}
