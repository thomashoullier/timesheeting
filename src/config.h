#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

struct UserConfig {
  std::filesystem::path log_filepath;
  std::vector<std::string> log_levels_to_activate;
};

/** @brief Config file loader class. */
class ConfigLoader {
public:
  /** @brief Load the configuration file into an internal representation. */
  UserConfig load(const std::filesystem::path &config_file);

private:
  /** @brief Expand tilde and braces/variables in file paths. */
  std::filesystem::path expand_tilde(const std::filesystem::path &path);
  /** @brief Parse a toml array into a string vector. */
  std::vector<std::string> parse_stringvec(toml::array *arr);
};

#endif // CONFIG_H
