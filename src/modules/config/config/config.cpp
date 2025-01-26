#include "config.h"
#include "config_lib/config_utils.h"
#include "config_lib/toml_loader.h"
#include "key.h"
#include "binding_maps.h"
#include <ncurses.h>
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
    auto max_log_age = config_loader->parse_unsigned({"log", "max_log_age"});
    auto bindings = parse_bindings(config_loader);
    return UserConfig(log_filepath,
                      db_filepath,
                      active_log_levels,
                      max_log_age,
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

  KeyBindings ConfigLoader::parse_bindings
  (std::shared_ptr<config_lib::TomlLoader> config_loader) {
    auto normal_map = parse_normalmode_bindings(config_loader);
    auto edit_map = parse_editmode_bindings(config_loader);
    return KeyBindings{normal_map, edit_map};
  }

  NormalMap ConfigLoader::parse_normalmode_bindings(
      std::shared_ptr<config_lib::TomlLoader> config_loader) {
    NormalMap map;
    set_key(map, config_loader,
                   {"keys", "navigation", "up"},
                   NormalActions::up);
    set_key(map, config_loader,
                   {"keys", "navigation", "down"},
                   NormalActions::down);
    set_key(map, config_loader,
                   {"keys", "navigation", "left"},
                   NormalActions::left);
    set_key(map, config_loader,
                   {"keys", "navigation", "right"},
                   NormalActions::right);
    set_key(map, config_loader,
                   {"keys", "navigation", "subtabs"},
                   NormalActions::subtabs);
    set_key(map, config_loader,
                   {"keys", "navigation", "previous"},
                   NormalActions::previous);
    set_key(map, config_loader,
                   {"keys", "navigation", "next"},
                   NormalActions::next);
    set_key(map, config_loader,
                   {"keys", "navigation", "duration_display"},
                   NormalActions::duration_display);
    set_key(map, config_loader,
                   {"keys", "navigation", "entries_screen"},
                   NormalActions::entries_screen);
    set_key(map, config_loader,
                   {"keys", "navigation", "projects_screen"},
                   NormalActions::projects_screen);
    set_key(map, config_loader,
                   {"keys", "navigation", "locations_screen"},
                   NormalActions::locations_screen);
    set_key(map, config_loader,
                   {"keys", "navigation", "project_report_screen"},
                   NormalActions::project_report_screen);
    set_key(map, config_loader,
            {"keys", "navigation", "weekly_report_screen"},
            NormalActions::weekly_report_screen);
    set_key(map, config_loader,
                   {"keys", "navigation", "active_visibility"},
                   NormalActions::active_visibility);
    set_key(map, config_loader,
                   {"keys", "navigation", "quit"},
                   NormalActions::quit);
    set_key(map, config_loader,
                   {"keys", "actions", "commit_entry"},
                   NormalActions::commit_entry);
    set_key(map, config_loader,
                   {"keys", "actions", "set_now"},
                   NormalActions::set_now);
    set_key(map, config_loader,
                   {"keys", "actions", "add"},
                   NormalActions::add);
    set_key(map, config_loader,
                   {"keys", "actions", "rename"},
                   NormalActions::rename);
    set_key(map, config_loader,
                   {"keys", "actions", "remove"},
                   NormalActions::remove);
    set_key(map, config_loader, {"keys", "actions", "active_toggle"},
                   NormalActions::active_toggle);
    set_key(map, config_loader, {"keys", "actions", "task_project_change"},
                   NormalActions::task_project_change);
    return map;
  }

  EditMap ConfigLoader::parse_editmode_bindings
    (std::shared_ptr<config_lib::TomlLoader> config_loader) {
    EditMap map;
    set_key(map, config_loader,
            {"keys", "edit_mode", "validate"},
            EditActions::validate);
    set_key(map, config_loader,
            {"keys", "edit_mode", "cancel"},
            EditActions::cancel);
    set_key(map, config_loader,
            {"keys", "edit_mode", "select_suggestion"},
            EditActions::select_suggestion);
    map.add_binding(127, EditActions::backspace);
    return map;
  }

  int ConfigLoader::parse_special_string (const std::string &str) {
    if (str == "ESCAPE") {
      return 27;
    }
    if (str == "ENTER") {
      return '\n';
    }
    if (str == "SPACE") {
      return ' ';
    }
    if (str == "TAB") {
      return '\t';
    }
    if (str == "UP") {
      return KEY_UP;
    }
    if (str == "DOWN") {
      return KEY_DOWN;
    }
    if (str == "LEFT") {
      return KEY_LEFT;
    }
    if (str == "RIGHT") {
      return KEY_RIGHT;
    }
    throw std::runtime_error("Invalid special string for key binding.");
  }

  int ConfigLoader::parse_regular_string (const std::string &str) {
    return str.at(0);
  }

  int ConfigLoader::parse_binding_string(const std::string &str) {
    auto len = str.length();
    if (len == 1) {
      return parse_regular_string(str);
    } else if (len > 1) {
      return parse_special_string(str);
    } else {
      throw std::runtime_error("Invalid string for key binding.");
    }
  }
} // namespace config
