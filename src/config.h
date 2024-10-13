#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

namespace config {
  struct UserConfig {
    std::filesystem::path log_filepath;
    std::vector<std::string> log_levels_to_activate;
  };

  /** @brief Config file loader class. */
  class ConfigLoader {
  public:
    /** @brief Load the configuration file into an internal representation. */
    UserConfig load(const std::filesystem::path &config_file);
    /** @brief Find and load the configuration file.

        The loader searches for `timesheeting/timesheeting.toml`,
        in priority in:
        1. $XDG_CONFIG_HOME/
        2. $HOME/.config/
        3. /etc/ */
    UserConfig load();

  private:
    /** @brief Expand tilde and braces/variables in file paths. */
    std::filesystem::path expand_tilde(const std::filesystem::path &path);
    /** @brief Parse a toml array into a string vector. */
    std::vector<std::string> parse_stringvec(toml::array *arr);
    /** @brief Search for the config file in default locations. */
    std::filesystem::path find_config_file ();
  };
}

#endif // CONFIG_H
