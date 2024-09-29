#include "config.h"
#include <filesystem>
#include <toml++/toml.hpp>

UserConfig ConfigLoader::load(const std::filesystem::path &config_file) {
  auto config = toml::parse_file(config_file.u8string());
  auto log_filepath = config["config"]["log_file"].value_or("");

  return UserConfig(log_filepath);
}
