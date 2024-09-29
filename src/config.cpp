#include "config.h"
#include <filesystem>
#include <toml++/toml.hpp>

UserConfig ConfigLoader::load(const std::filesystem::path &config_file) {
  auto config_path = expand_tilde(config_file);
  auto config = toml::parse_file(config_path.u8string());
  auto log_filepath = config["config"]["log_file"].value_or("");
  return UserConfig(expand_tilde(log_filepath));
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
