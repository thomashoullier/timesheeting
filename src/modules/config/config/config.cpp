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
      std::filesystem::path temp_path = std::getenv("HOME");
      temp_path /= ".config";
      try_paths.push_back(temp_path);
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
    set_key(kb.navigation.up, config_loader, {"keys", "navigation", "up"});
    set_key(kb.navigation.down, config_loader, {"keys", "navigation", "down"});
    set_key(kb.navigation.left, config_loader, {"keys", "navigation", "left"});
    set_key(kb.navigation.right, config_loader, {"keys", "navigation", "right"});
    set_key(kb.navigation.subtabs, config_loader,
            {"keys", "navigation", "subtabs"});
    set_key(kb.navigation.previous, config_loader,
            {"keys", "navigation", "previous"});
    set_key(kb.navigation.next, config_loader,
            {"keys", "navigation", "next"});
    set_key(kb.navigation.duration_display, config_loader,
            {"keys", "navigation", "duration_display"});
    set_key(kb.navigation.entries_screen, config_loader,
            {"keys", "navigation", "entries_screen"});
    set_key(kb.navigation.projects_screen, config_loader,
            {"keys", "navigation", "projects_screen"});
    set_key(kb.navigation.locations_screen, config_loader,
            {"keys", "navigation", "locations_screen"});
    set_key(kb.navigation.project_report_screen, config_loader,
            {"keys", "navigation", "project_report_screen"});
    set_key(kb.navigation.weekly_report_screen, config_loader,
            {"keys", "navigation", "weekly_report_screen"});
    set_key(kb.navigation.active_visibility, config_loader,
            {"keys", "navigation", "active_visibility"});
    set_key(kb.navigation.quit, config_loader,
            {"keys", "navigation", "quit"});
    set_key(kb.actions.commit_entry, config_loader,
            {"keys", "actions", "commit_entry"});
    set_key(kb.actions.set_now, config_loader, {"keys", "actions", "set_now"});
    set_key(kb.actions.add, config_loader, {"keys", "actions", "add"});
    set_key(kb.actions.rename, config_loader, {"keys", "actions", "rename"});
    set_key(kb.actions.remove, config_loader, {"keys", "actions", "remove"});
    set_key(kb.actions.active_toggle, config_loader,
            {"keys", "actions", "active_toggle"});
    set_key(kb.actions.task_project_change, config_loader,
            {"keys", "actions", "task_project_change"});
    set_key(kb.edit_mode.validate, config_loader,
            {"keys", "edit_mode", "validate"});
    set_key(kb.edit_mode.cancel, config_loader,
            {"keys", "edit_mode", "cancel"});
    set_key(kb.edit_mode.select_suggestion, config_loader,
            {"keys", "edit_mode", "select_suggestion"});
    return kb;
  }

  char ConfigLoader::parse_binding_string(const std::string &str) {
    // Parsing special keys
    if (str == "ESCAPE")
      return 27;
    // Parsing regular keys
    if (str.length() != 1)
      throw std::runtime_error("Invalid string for key binding.");
    return str.at(0);
  }

  void
  ConfigLoader::set_key(Key &k,
                        std::shared_ptr<config_lib::TomlLoader> config_loader,
                        std::vector<std::string> tree_pos) {
    // Primary binding
    auto str = config_loader->parse_string(tree_pos);
    k.primary = parse_binding_string(str);
    // Alternate binding
    tree_pos.back() += "_alt";
    if (config_loader->node_exists(tree_pos)) {
      str = config_loader->parse_string(tree_pos);
      k.alt = parse_binding_string(str);
    }
  }
} // namespace config
