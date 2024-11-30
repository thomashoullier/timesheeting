#include "config.h"
#include "config_lib/config_utils.h"
#include <cmath>
#include <filesystem>
#include <stdexcept>
#include <limits>

namespace config {
  UserConfig ConfigLoader::load(const std::filesystem::path &config_file) {
    auto config_path = config_lib::expand_tilde(config_file);
    if (not(std::filesystem::exists(config_path)))
      throw std::runtime_error("The configuration file \"" + config_path.string()
                               + "\" does not exist.");
    auto config = toml::parse_file(config_path.u8string());
    auto db_filepath = parse_filepath(config["db"]["file"]);
    auto timezone = parse_string(config["time"]["timezone"]);
    auto hours_per_day = parse_float(config["time"]["hours_per_workday"]);
    auto log_filepath = parse_filepath(config["log"]["file"]);
    auto active_log_levels = parse_stringvec
      (config["log"]["active_levels"]);
    auto bindings = parse_bindings(config["keys"]);
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

  std::string ConfigLoader::parse_string
  (const toml::node_view<toml::node> &config_node) {
    std::string str = config_node.value_or(std::string{});
    if (str.empty()) {
      throw std::runtime_error("String read from the configuration is empty.");
    }
    return str;
  }

  float ConfigLoader::parse_float
  (const toml::node_view<toml::node> &config_node) {
    float num = config_node.value_or(std::numeric_limits<float>::quiet_NaN());
    if (std::isnan(num)) {
      throw std::runtime_error("Float read from the configuration is empty.");
    }
    return num;
  }

  std::filesystem::path
  ConfigLoader::parse_filepath(const toml::node_view<toml::node> &config_node) {
    std::filesystem::path path = parse_string(config_node);
    path = config_lib::expand_tilde(path);
    if (not std::filesystem::exists(path.parent_path()))
      throw std::runtime_error("Provided filepath does not exist: " +
                               std::string(path.parent_path()));
    return path;
  }

  std::vector<std::string> ConfigLoader::parse_stringvec
  (const toml::node_view<toml::node> &config_node) {
    std::vector<std::string> strings;
    config_node.as_array()->for_each([&strings](auto &&el) {
      if constexpr (toml::is_string<decltype(el)>)
        strings.push_back(std::string(el));
      else
        throw std::runtime_error("Invalid configuration array type: should be"
                                 " a string.");
    });
    return strings;
  }

  KeyBindings ConfigLoader::parse_bindings
  (const toml::node_view<toml::node> &keys_node) {
    KeyBindings kb;
    kb.up.primary = parse_key(keys_node["up"]);
    kb.down.primary = parse_key(keys_node["down"]);
    kb.left.primary = parse_key(keys_node["left"]);
    kb.right.primary = parse_key(keys_node["right"]);
    kb.subtabs.primary = parse_key(keys_node["subtabs"]);
    kb.previous.primary = parse_key(keys_node["previous"]);
    kb.next.primary = parse_key(keys_node["next"]);
    kb.set_now.primary = parse_key(keys_node["set_now"]);
    kb.validate.primary = parse_key(keys_node["validate"]);
    kb.cancel.primary = parse_key(keys_node["cancel"]);
    kb.select_suggestion.primary = parse_key(keys_node["select_suggestion"]);
    kb.duration_display.primary = parse_key(keys_node["duration_display"]);
    kb.entries_screen.primary = parse_key(keys_node["entries_screen"]);
    kb.projects_screen.primary = parse_key(keys_node["projects_screen"]);
    kb.locations_screen.primary = parse_key(keys_node["locations_screen"]);
    kb.project_report_screen.primary =
      parse_key(keys_node["project_report_screen"]);
    kb.weekly_report_screen.primary =
      parse_key(keys_node["weekly_report_screen"]);
    kb.add.primary = parse_key(keys_node["add"]);
    kb.rename.primary = parse_key(keys_node["rename"]);
    kb.remove.primary = parse_key(keys_node["remove"]);
    kb.active_toggle.primary = parse_key(keys_node["active_toggle"]);
    kb.active_visibility.primary = parse_key(keys_node["active_visibility"]);
    kb.task_project_change.primary =
      parse_key(keys_node["task_project_change"]);
    kb.quit.primary = parse_key(keys_node["quit"]);
    return kb;
  }

  char ConfigLoader::parse_key (const toml::node_view<toml::node> &keys_node) {
    auto str = parse_string(keys_node);
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
