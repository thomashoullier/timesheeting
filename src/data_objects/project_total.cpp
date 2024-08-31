#include "project_total.h"

std::vector<std::string> ProjectTotal::to_strings() const {
  std::vector<std::string> display_strings(2);
  display_strings.at(0) = project_name;
  display_strings.at(1) = total.to_string();
  return display_strings;
}
