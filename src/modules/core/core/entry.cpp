#include "entry.h"

namespace core {
  std::vector<std::string> Entry::to_strings() const {
    std::vector<std::string> display_strings(5);
    display_strings.at(0) = project_name;
    display_strings.at(1) = task_name;
    display_strings.at(2) = start.to_string();
    display_strings.at(3) = stop.to_string();
    display_strings.at(4) = location_name;
    return display_strings;
  }

  std::vector<std::string> Entry::to_shortstrings() const {
    std::vector<std::string> display_strings(5);
    display_strings.at(0) = project_name;
    display_strings.at(1) = task_name;
    display_strings.at(2) = start.to_shortstring();
    display_strings.at(3) = stop.to_shortstring();
    display_strings.at(4) = location_name;
    return display_strings;
  }
}
