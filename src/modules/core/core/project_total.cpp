#include "project_total.h"
#include "time_lib/duration_displayer.h"

namespace core {
  std::vector<std::string> ProjectTotal::to_strings() const {
    std::vector<std::string> display_strings(2);
    display_strings.at(0) = project_name;
    display_strings.at(1) = time_lib::DurationDisplayer::get().to_string(total);
    return display_strings;
  }
}
