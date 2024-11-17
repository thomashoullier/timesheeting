#include "config.h"
#include <cmath>
#include <filesystem>
#include <stdexcept>
#include <limits>

namespace config {
  UserConfig ConfigLoader::load(const std::filesystem::path &config_file) {
    auto config_path = expand_tilde(config_file);
    auto config = toml::parse_file(config_path.u8string());
    auto log_filepath = parse_filepath(config["config"]["log_file"]);
    auto db_filepath = parse_filepath(config["config"]["db_file"]);
    auto active_log_levels = parse_stringvec
      (config["config"]["active_log_levels"]);
    auto timezone = parse_string(config["config"]["timezone"]);
    auto hours_per_day = parse_float(config["config"]["hours_per_workday"]);
    auto bindings = parse_bindings(config["keys"]);
    return UserConfig(log_filepath,
                      db_filepath,
                      active_log_levels,
                      timezone,
                      hours_per_day,
                      bindings);
  }

  UserConfig ConfigLoader::load() {
    auto config_filepath = find_config_file();
    return load(config_filepath);
  }

  std::filesystem::path
  ConfigLoader::expand_tilde(const std::filesystem::path &path) {
    std::string str = path;
    if (str.starts_with("~/")) {
      std::filesystem::path home_dir = std::getenv("HOME");
      return home_dir / str.substr(2);
    }
    return path;
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
    path = expand_tilde(path);
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

  std::filesystem::path ConfigLoader::find_config_file() {
    std::filesystem::path path;
    std::filesystem::path suffix = "timesheeting/timesheeting.toml";
    if (std::getenv("XDG_CONFIG_HOME")) {
      path = std::getenv("XDG_CONFIG_HOME");
      path /= suffix;
      if (std::filesystem::exists(path))
        return path;
    }
    if (std::getenv("HOME")) {
      path = std::getenv("HOME");
      path /= suffix;
      if (std::filesystem::exists(path))
        return path;
    }
    path = "/etc/";
    path /= suffix;
    if (std::filesystem::exists(path)) {
      return path;
    }
    throw std::runtime_error("No configuration file was found.");
  }
  } // namespace config
