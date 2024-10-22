#include "project_total.h"
#include "../duration_displayer.h"

std::vector<std::string> ProjectTotal::to_strings() const {
  std::vector<std::string> display_strings(2);
  display_strings.at(0) = project_name;
  display_strings.at(1) = DurationDisplayer::get().to_string(total);
  return display_strings;
}
