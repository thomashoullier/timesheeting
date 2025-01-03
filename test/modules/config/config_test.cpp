#include "config_test.h"
#include "test_utils/test_utils.h"
#include "config/config.h"
#include <catch2/catch_test_macros.hpp>
#include <fstream>

std::filesystem::path
generate_example_config(const std::filesystem::path &dirpath) {
  const std::string content =
    "[db]\n"
    "file = \"~/.local/timesheeting/timesheeting.db\"\n"
    "\n"
    "[time]\n"
    "timezone = \"Europe/Paris\"\n"
    "hours_per_workday = 7.7\n"
    "\n"
    "[log]\n"
    "file = \"~/.local/timesheeting/timesheeting.log\"\n"
    "active_levels = [ \"debug\", \"error\", \"info\" ]\n"
    "\n"
    "[keys]\n"
    "  [keys.navigation]\n"
    "  up = \"e\"\n"
    "  down = \"n\"\n"
    "  left = \"h\"\n"
    "  right = \"i\"\n"
    "  subtabs = \"\t\"\n"
    "  previous = \",\"\n"
    "  next = \".\"\n"
    "  duration_display = \"d\"\n"
    "  entries_screen = \"1\"\n"
    "  projects_screen = \"2\"\n"
    "  locations_screen = \"3\"\n"
    "  project_report_screen = \"4\"\n"
    "  weekly_report_screen = \"5\"\n"
    "  active_visibility = \".\"\n"
    "  quit = \"q\"\n"
    "  [keys.actions]\n"
    "  commit_entry = \"\\n\"\n"
    "  set_now = \" \"\n"
    "  add = \"a\"\n"
    "  rename = \"r\"\n"
    "  remove = \"x\"\n"
    "  active_toggle = \"b\"\n"
    "  task_project_change = \"p\"\n"
    "  [keys.edit_mode]\n"
    "  validate = \"\\n\"\n"
    "  cancel = \"ESCAPE\"\n"
    "  select_suggestion = \"\t\"\n";
  const std::filesystem::path filepath{dirpath / "config_file.toml"};
  std::ofstream out {filepath};
  out << content;
  return filepath;
}

TEST_CASE("MT-CON-010 Config file loading", "[config]") {
  auto dir = test_utils::TempDir("timesheeting_config_lib_test");
  auto filepath = generate_example_config(dir.dirpath);
  auto config = config::ConfigLoader().load(filepath);
  SUCCEED("Configuration file loaded without error");
}

TEST_CASE("MT-CON-020 Config file non-existent", "[config]") {
  std::filesystem::path nonexistent_path = "/dev/null/nonexistent";
  CHECK_THROWS(config::ConfigLoader().load(nonexistent_path));
}
