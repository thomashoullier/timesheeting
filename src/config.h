#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <string>
#include <toml++/toml.hpp>
#include <vector>
#include "ui/keys/key.h"

namespace config {
  struct UserConfig {
    std::filesystem::path log_filepath;
    std::filesystem::path db_filepath;
    std::vector<std::string> log_levels_to_activate;
    std::string timezone;
    float hours_per_workday;
    keys::KeyBindings bindings;
  };

  /** @brief Config file loader class. */
  class ConfigLoader {
    // TODO: Make sure the parsed filepaths actually exist.
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
    /** @brief Parse a string from the configuration.

        Empty strings are forbidden and raise an exception. */
    std::string parse_string
    (const toml::node_view<toml::node> &config_node);
    /** @brief Parse a float number from the configuration. */
    float parse_float(const toml::node_view<toml::node> &config_node);
    /** @brief Parse a filepath from the configuration. */
    std::filesystem::path parse_filepath(
        const toml::node_view<toml::node> &config_node);
    /** @brief Parse a toml array into a string vector. */
    std::vector<std::string> parse_stringvec(
        const toml::node_view<toml::node> &config_node);
    /** @brief Parse key bindings. */
    keys::KeyBindings parse_bindings
      (const toml::node_view<toml::node> &keys_node);
    /** @brief Parse a key. */
    char parse_key (const toml::node_view<toml::node> &key_node);
    /** @brief Search for the config file in default locations. */
    std::filesystem::path find_config_file();
  };
} // namespace config

#endif // CONFIG_H
