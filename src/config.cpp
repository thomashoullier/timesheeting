#include "config.h"
#include <filesystem>
#include <stdexcept>

UserConfig ConfigLoader::load(const std::filesystem::path &config_file) {
  auto config_path = expand_tilde(config_file);
  auto config = toml::parse_file(config_path.u8string());
  auto log_filepath = config["config"]["log_file"].value_or("");
  auto active_log_levels = config["config"]["active_log_levels"];
  return UserConfig(expand_tilde(log_filepath),
                    parse_stringvec(active_log_levels.as_array()));
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

std::vector<std::string> ConfigLoader::parse_stringvec(toml::array *arr) {
  std::vector<std::string> strings;
  arr->for_each([&strings](auto&& el) {
    if constexpr (toml::is_string<decltype(el)>)
      strings.push_back(std::string(el));
    else
      throw std::runtime_error("Invalid configuration array type: should be"
                               " a string.");
  });
  return strings;
}
