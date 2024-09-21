#include "entry_staging.h"

std::vector<std::string> EntryStaging::to_strings() const {
  std::vector<std::string> display_strings(5);
  display_strings.at(0) = project_name.value_or(" ");
  display_strings.at(1) = task_name.value_or(" ");
  display_strings.at(2) = start.has_value() ? start.value().to_string() : " ";
  display_strings.at(3) = stop.has_value() ? stop.value().to_string() : " ";
  display_strings.at(4) = location_name.value_or(" ");
  return display_strings;
}

std::vector<std::string> EntryStaging::to_shortstrings() const {
  std::vector<std::string> display_strings(5);
  display_strings.at(0) = project_name.value_or(" ");
  display_strings.at(1) = task_name.value_or(" ");
  display_strings.at(2) =
      start.has_value() ? start.value().to_shortstring() : " ";
  display_strings.at(3) =
      stop.has_value() ? stop.value().to_shortstring() : " ";
  display_strings.at(4) = location_name.value_or(" ");
  return display_strings;
}
