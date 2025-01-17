#include "config_test.h"
#include "config/key.h"
#include "test_utils/test_utils.h"
#include "config/config.h"
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <type_traits>
#include <ncurses.h>

std::filesystem::path
generate_example_config(const std::filesystem::path &dirpath) {
  const std::string content =
    "[db]\n"
    "file = \"/tmp/timesheeting.db\"\n"
    "\n"
    "[time]\n"
    "timezone = \"Europe/Paris\"\n"
    "hours_per_workday = 7.7\n"
    "\n"
    "[log]\n"
    "file = \"/tmp/timesheeting.log\"\n"
    "active_levels = [ \"debug\", \"error\", \"info\" ]\n"
    "\n"
    "[keys]\n"
    "[keys.navigation]\n"
    "up = [\"e\", \"UP\"]\n"
    "down = [\"n\", \"DOWN\"]\n"
    "left = [\"h\", \"LEFT\"]\n"
    "right = [\"i\", \"RIGHT\"]\n"
    "subtabs = [\"TAB\"]\n"
    "previous = [\",\"]\n"
    "next = [\".\"]\n"
    "duration_display = [\"d\"]\n"
    "entries_screen = [\"1\"]\n"
    "projects_screen = [\"2\"]\n"
    "locations_screen = [\"3\"]\n"
    "project_report_screen = [\"4\"]\n"
    "weekly_report_screen = [\"5\"]\n"
    "active_visibility = [\"v\"]\n"
    "quit = [\"q\"]\n"
    "[keys.actions]\n"
    "commit_entry = [\"ENTER\"]\n"
    "set_now = [\"SPACE\"]\n"
    "add = [\"a\"]\n"
    "rename = [\"r\"]\n"
    "remove = [\"x\"]\n"
    "active_toggle = [\"b\"]\n"
    "task_project_change = [\"p\"]\n"
    "[keys.edit_mode]\n"
    "validate = [\"ENTER\"]\n"
    "cancel = [\"ESCAPE\"]\n"
    "select_suggestion = [\"TAB\"]\n";

  const std::filesystem::path filepath{dirpath / "config_file.toml"};
  std::ofstream out {filepath};
  out << content;
  return filepath;
}

std::filesystem::path
generate_example_config_duplicate(const std::filesystem::path &dirpath) {
  const std::string content =
    "[db]\n"
    "file = \"/tmp/timesheeting.db\"\n"
    "\n"
    "[time]\n"
    "timezone = \"Europe/Paris\"\n"
    "hours_per_workday = 7.7\n"
    "\n"
    "[log]\n"
    "file = \"/tmp/timesheeting.log\"\n"
    "active_levels = [ \"debug\", \"error\", \"info\" ]\n"
    "\n"
    "[keys]\n"
    "[keys.navigation]\n"
    "up = [\"e\", \"UP\"]\n"
    "down = [\"n\", \"DOWN\"]\n"
    "left = [\"h\", \"LEFT\"]\n"
    "right = [\"i\", \"RIGHT\"]\n"
    "subtabs = [\"TAB\"]\n"
    "previous = [\",\"]\n"
    "next = [\".\"]\n"
    "duration_display = [\"d\"]\n"
    "entries_screen = [\"1\"]\n"
    "projects_screen = [\"2\"]\n"
    "locations_screen = [\"3\"]\n"
    "project_report_screen = [\"4\"]\n"
    "weekly_report_screen = [\"5\"]\n"
    "active_visibility = [\"v\"]\n"
    "quit = [\"q\"]\n"
    "[keys.actions]\n"
    "commit_entry = [\"ENTER\"]\n"
    "set_now = [\"SPACE\"]\n"
    "add = [\"a\", \"v\"]\n"
    "rename = [\"r\"]\n"
    "remove = [\"x\"]\n"
    "active_toggle = [\"b\"]\n"
    "task_project_change = [\"p\"]\n"
    "[keys.edit_mode]\n"
    "validate = [\"ENTER\"]\n"
    "cancel = [\"ESCAPE\"]\n"
    "select_suggestion = [\"TAB\"]\n";

  const std::filesystem::path filepath{dirpath / "config_file.toml"};
  std::ofstream out {filepath};
  out << content;
  return filepath;
}

std::filesystem::path
generate_example_config_invalidstr(const std::filesystem::path &dirpath) {
  const std::string content =
    "[db]\n"
    "file = \"/tmp/timesheeting.db\"\n"
    "\n"
    "[time]\n"
    "timezone = \"Europe/Paris\"\n"
    "hours_per_workday = 7.7\n"
    "\n"
    "[log]\n"
    "file = \"/tmp/timesheeting.log\"\n"
    "active_levels = [ \"debug\", \"error\", \"info\" ]\n"
    "\n"
    "[keys]\n"
    "[keys.navigation]\n"
    "up = [\"e\", \"UP\", \"GOOFY\"]\n"
    "down = [\"n\", \"DOWN\"]\n"
    "left = [\"h\", \"LEFT\"]\n"
    "right = [\"i\", \"RIGHT\"]\n"
    "subtabs = [\"TAB\"]\n"
    "previous = [\",\"]\n"
    "next = [\".\"]\n"
    "duration_display = [\"d\"]\n"
    "entries_screen = [\"1\"]\n"
    "projects_screen = [\"2\"]\n"
    "locations_screen = [\"3\"]\n"
    "project_report_screen = [\"4\"]\n"
    "weekly_report_screen = [\"5\"]\n"
    "active_visibility = [\"v\"]\n"
    "quit = [\"q\"]\n"
    "[keys.actions]\n"
    "commit_entry = [\"ENTER\"]\n"
    "set_now = [\"SPACE\"]\n"
    "add = [\"a\"]\n"
    "rename = [\"r\"]\n"
    "remove = [\"x\"]\n"
    "active_toggle = [\"b\"]\n"
    "task_project_change = [\"p\"]\n"
    "[keys.edit_mode]\n"
    "validate = [\"ENTER\"]\n"
    "cancel = [\"ESCAPE\"]\n"
    "select_suggestion = [\"TAB\"]\n";

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

TEST_CASE("Configuration contents", "[config]") {
  auto dir = test_utils::TempDir("timesheeting_config_lib_test");
  auto filepath = generate_example_config(dir.dirpath);
  auto config = config::ConfigLoader().load(filepath);
  SECTION("MT-CON-030 Log filepath") {
    CHECK(std::is_same<decltype(config.log_filepath),
          std::filesystem::path>());
    CHECK(not(config.log_filepath.empty()));
  }
  SECTION("MT-CON-040 Log levels") {
    CHECK(std::is_same<decltype(config.log_levels_to_activate),
                       std::vector<std::string>>());
  }
  SECTION("MT-CON-050 DB filepath") {
    CHECK(std::is_same<decltype(config.db_filepath), std::filesystem::path>());
    CHECK(not(config.db_filepath.empty()));
  }
  SECTION("MT-CON-060 Timezone string") {
    CHECK(std::is_same<decltype(config.timezone), std::string>());
    CHECK(not(config.timezone.empty()));
  }
  SECTION("MT-CON-070 Duration display days") {
    CHECK(std::is_same<decltype(config.hours_per_workday), float>());
  }
  SECTION("MT-CON-080 Bindings loading") {
    CHECK(config.bindings.normal_mode.action_requested('e') ==
          config::NormalActions::up);
    CHECK(config.bindings.normal_mode.action_requested(KEY_UP) ==
          config::NormalActions::up);
    CHECK(config.bindings.normal_mode.action_requested('n') ==
          config::NormalActions::down);
    CHECK(config.bindings.normal_mode.action_requested(KEY_DOWN) ==
          config::NormalActions::down);
    CHECK(config.bindings.normal_mode.action_requested('h') ==
          config::NormalActions::left);
    CHECK(config.bindings.normal_mode.action_requested(KEY_LEFT) ==
          config::NormalActions::left);
    CHECK(config.bindings.normal_mode.action_requested('i') ==
          config::NormalActions::right);
    CHECK(config.bindings.normal_mode.action_requested(KEY_RIGHT) ==
          config::NormalActions::right);
    CHECK(config.bindings.normal_mode.action_requested('\t') ==
          config::NormalActions::subtabs);
    CHECK(config.bindings.normal_mode.action_requested('\t') ==
          config::NormalActions::subtabs);
    CHECK(config.bindings.normal_mode.action_requested(',') ==
          config::NormalActions::previous);
    CHECK(config.bindings.normal_mode.action_requested('.') ==
          config::NormalActions::next);
    CHECK(config.bindings.normal_mode.action_requested('d') ==
          config::NormalActions::duration_display);
    CHECK(config.bindings.normal_mode.action_requested('1') ==
          config::NormalActions::entries_screen);
    CHECK(config.bindings.normal_mode.action_requested('2') ==
          config::NormalActions::projects_screen);
    CHECK(config.bindings.normal_mode.action_requested('3') ==
          config::NormalActions::locations_screen);
    CHECK(config.bindings.normal_mode.action_requested('4') ==
          config::NormalActions::project_report_screen);
    CHECK(config.bindings.normal_mode.action_requested('5') ==
          config::NormalActions::weekly_report_screen);
    CHECK(config.bindings.normal_mode.action_requested('v') ==
          config::NormalActions::active_visibility);
    CHECK(config.bindings.normal_mode.action_requested('q') ==
          config::NormalActions::quit);
    CHECK(config.bindings.normal_mode.action_requested('\n') ==
          config::NormalActions::commit_entry);
    CHECK(config.bindings.normal_mode.action_requested(' ') ==
          config::NormalActions::set_now);
    CHECK(config.bindings.normal_mode.action_requested('a') ==
          config::NormalActions::add);
    CHECK(config.bindings.normal_mode.action_requested('r') ==
          config::NormalActions::rename);
    CHECK(config.bindings.normal_mode.action_requested('x') ==
          config::NormalActions::remove);
    CHECK(config.bindings.normal_mode.action_requested('b') ==
          config::NormalActions::active_toggle);
    CHECK(config.bindings.normal_mode.action_requested('p') ==
          config::NormalActions::task_project_change);
    CHECK(config.bindings.edit_mode.action_requested('\n') ==
          config::EditActions::validate);
    CHECK(config.bindings.edit_mode.action_requested(27) ==
          config::EditActions::cancel);
    CHECK(config.bindings.edit_mode.action_requested('\t') ==
          config::EditActions::select_suggestion);
  }
  SECTION("MT-CON-100 Backspace mapping") {
    CHECK(config.bindings.edit_mode.action_requested(127) ==
          config::EditActions::backspace);
  }
  SECTION("MT-CON-110 Unbound mappings") {
    CHECK(config.bindings.normal_mode.action_requested('u') ==
          config::NormalActions::unbound);
    CHECK(config.bindings.edit_mode.action_requested('u') ==
          config::EditActions::unbound);
  }
}

TEST_CASE("MT-CON-090 Binding duplicate", "[config]") {
  auto dir = test_utils::TempDir("timesheeting_config_lib_test");
  auto filepath = generate_example_config_duplicate(dir.dirpath);
  CHECK_THROWS(config::ConfigLoader().load(filepath));
}

TEST_CASE("MT-CON-120 Invalid special key", "[config]") {
  auto dir = test_utils::TempDir("timesheeting_config_lib_test");
  auto filepath = generate_example_config_invalidstr(dir.dirpath);
  CHECK_THROWS(config::ConfigLoader().load(filepath));
}
