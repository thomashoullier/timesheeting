#include "config.h"
#include <filesystem>
#include <stdexcept>

namespace config {
  UserConfig ConfigLoader::load(const std::filesystem::path &config_file) {
    auto config_path = expand_tilde(config_file);
    auto config = toml::parse_file(config_path.u8string());
    auto log_filepath = parse_filepath(config["config"]["log_file"]);
    auto active_log_levels = parse_stringvec
      (config["config"]["active_log_levels"]);
    return UserConfig(log_filepath,
                      active_log_levels);
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

  std::filesystem::path
  ConfigLoader::parse_filepath(const toml::node_view<toml::node> &config_node) {
    std::string str = config_node.value_or(std::string{});
    if (str.empty()) {
      throw std::runtime_error("Could not read filepath from configuration.");
    }
    std::filesystem::path path = str;
    path = expand_tilde(path);
    if (not std::filesystem::exists(path))
      throw std::runtime_error("Provided filepath does not exist: " +
                               std::string(path));
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
}
