#include "config.h"
#include "config_lib/config_utils.h"
#include "config_lib/toml_loader.h"
#include <cmath>
#include <filesystem>
#include <memory>
#include <stdexcept>

namespace config {
  UserConfig ConfigLoader::load(const std::filesystem::path &config_file) {
    auto config_path = config_lib::expand_tilde(config_file);
    if (not(std::filesystem::exists(config_path)))
      throw std::runtime_error("The configuration file \"" + config_path.string()
                               + "\" does not exist.");
    auto config_loader =
      std::make_shared<config_lib::TomlLoader>
      (config_lib::TomlLoader(config_path));
    auto db_filepath = config_loader->parse_filepath({"db", "file"});
    auto timezone = config_loader->parse_string({"time", "timezone"});
    auto hours_per_day =
      config_loader->parse_float({"time", "hours_per_workday"});
    auto log_filepath = config_loader->parse_filepath({"log", "file"});
    auto active_log_levels =
      config_loader->parse_stringvec({"log", "active_levels"});
    auto bindings = parse_bindings(config_loader);
    return UserConfig(log_filepath,
                      db_filepath,
                      active_log_levels,
                      timezone,
                      hours_per_day,
                      bindings);
  }

  UserConfig ConfigLoader::load() {
    std::vector<std::filesystem::path> try_paths;
    if (std::getenv("XDG_CONFIG_HOME")) {
      try_paths.push_back(std::getenv("XDG_CONFIG_HOME"));
    }
    if (std::getenv("HOME")) {
      try_paths.push_back(std::getenv("HOME"));
    }
    try_paths.push_back("/etc/");
    auto maybe_config_filepath =
        config_lib::find_file(try_paths, "timesheeting/timesheeting.toml");
    if (maybe_config_filepath.has_value()) {
      return load(maybe_config_filepath.value());
    } else {
      throw std::runtime_error("No configuration file was found.");
    }
  }

  KeyBindings ConfigLoader::parse_bindings(
      std::shared_ptr<config_lib::TomlLoader> config_loader) {
    KeyBindings kb;
    kb.up.primary = parse_key(config_loader, {"keys", "up"});
    kb.down.primary = parse_key(config_loader, {"keys", "down"});
    kb.left.primary = parse_key(config_loader, {"keys", "left"});
    kb.right.primary = parse_key(config_loader, {"keys", "right"});
    kb.subtabs.primary = parse_key(config_loader, {"keys", "subtabs"});
    kb.previous.primary = parse_key(config_loader, {"keys", "previous"});
    kb.next.primary = parse_key(config_loader, {"keys", "next"});
    kb.set_now.primary = parse_key(config_loader, {"keys", "set_now"});
    kb.validate.primary = parse_key(config_loader, {"keys", "validate"});
    kb.commit_entry.primary = parse_key(config_loader, {"keys", "commit_entry"});
    kb.cancel.primary = parse_key(config_loader, {"keys", "cancel"});
    kb.select_suggestion.primary =
      parse_key(config_loader, {"keys", "select_suggestion"});
    kb.duration_display.primary =
      parse_key(config_loader, {"keys", "duration_display"});
    kb.entries_screen.primary =
      parse_key(config_loader, {"keys", "entries_screen"});
    kb.projects_screen.primary =
      parse_key(config_loader, {"keys", "projects_screen"});
    kb.locations_screen.primary =
      parse_key(config_loader, {"keys", "locations_screen"});
    kb.project_report_screen.primary =
      parse_key(config_loader, {"keys", "project_report_screen"});
    kb.weekly_report_screen.primary =
      parse_key(config_loader, {"keys", "weekly_report_screen"});
    kb.add.primary = parse_key(config_loader, {"keys", "add"});
    kb.rename.primary = parse_key(config_loader, {"keys", "rename"});
    kb.remove.primary = parse_key(config_loader, {"keys", "remove"});
    kb.active_toggle.primary =
      parse_key(config_loader, {"keys", "active_toggle"});
    kb.active_visibility.primary =
      parse_key(config_loader, {"keys", "active_visibility"});
    kb.task_project_change.primary =
      parse_key(config_loader, {"keys", "task_project_change"});
    kb.quit.primary = parse_key(config_loader, {"keys", "quit"});
    return kb;
  }

  char ConfigLoader::parse_key
  (std::shared_ptr<config_lib::TomlLoader> config_loader,
   const std::vector<std::string> &tree_pos) {
    auto str = config_loader->parse_string(tree_pos);
    // Parsing special keys
    if (str == "ESCAPE")
      return 27;
    // Parsing regular keys
    if (str.length() != 1)
      throw std::runtime_error("Invalid string for key binding.");
    char c = str.at(0);
    return c;
  }
} // namespace config
