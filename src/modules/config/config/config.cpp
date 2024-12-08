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
    kb.up.primary = parse_key(config_loader,
                              {"keys", "navigation", "up"});
    kb.down.primary = parse_key(config_loader,
                                {"keys", "navigation", "down"});
    kb.left.primary = parse_key(config_loader,
                                {"keys", "navigation", "left"});
    kb.right.primary = parse_key(config_loader,
                                 {"keys", "navigation", "right"});
    kb.subtabs.primary = parse_key(config_loader,
                                   {"keys", "navigation", "subtabs"});
    kb.previous.primary = parse_key(config_loader,
                                    {"keys", "navigation", "previous"});
    kb.next.primary = parse_key(config_loader,
                                {"keys", "navigation", "next"});
    kb.duration_display.primary =
      parse_key(config_loader, {"keys", "navigation", "duration_display"});
    kb.entries_screen.primary =
      parse_key(config_loader, {"keys", "navigation", "entries_screen"});
    kb.projects_screen.primary =
      parse_key(config_loader, {"keys", "navigation", "projects_screen"});
    kb.locations_screen.primary =
      parse_key(config_loader, {"keys", "navigation", "locations_screen"});
    kb.project_report_screen.primary =
      parse_key(config_loader, {"keys", "navigation", "project_report_screen"});
    kb.weekly_report_screen.primary =
      parse_key(config_loader, {"keys", "navigation", "weekly_report_screen"});
    kb.active_visibility.primary =
      parse_key(config_loader, {"keys", "navigation", "active_visibility"});
    kb.quit.primary = parse_key(config_loader, {"keys", "navigation", "quit"});
    kb.commit_entry.primary =
      parse_key(config_loader, {"keys", "actions", "commit_entry"});
    kb.set_now.primary =
      parse_key(config_loader, {"keys", "actions", "set_now"});
    kb.add.primary = parse_key(config_loader, {"keys", "actions", "add"});
    kb.rename.primary = parse_key(config_loader, {"keys", "actions", "rename"});
    kb.remove.primary = parse_key(config_loader, {"keys", "actions", "remove"});
    kb.active_toggle.primary =
      parse_key(config_loader, {"keys", "actions", "active_toggle"});
    kb.task_project_change.primary =
      parse_key(config_loader, {"keys", "actions", "task_project_change"});
    kb.validate.primary =
      parse_key(config_loader, {"keys", "edit_mode", "validate"});
    kb.cancel.primary =
      parse_key(config_loader, {"keys", "edit_mode", "cancel"});
    kb.select_suggestion.primary =
      parse_key(config_loader, {"keys", "edit_mode", "select_suggestion"});
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
