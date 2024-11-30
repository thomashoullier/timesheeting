/** @file
 *  @brief Managing the configuration loading. */
#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <string>
#include <toml++/toml.hpp>
#include <vector>
#include "key.h"

namespace config {
  /** @brief Set of data produced by the ConfigLoader. */
  struct UserConfig {
    /** @brief Path to the log file. */
    std::filesystem::path log_filepath;
    /** @brief Path to the DB file. */
    std::filesystem::path db_filepath;
    /** @brief Set of log levels to activate, by name. */
    std::vector<std::string> log_levels_to_activate;
    /** @brief Timezone to set in the program, TZ string format. */
    std::string timezone;
    /** @brief Number of hours in a workday. For displaying durations as days. */
    float hours_per_workday;
    /** @brief The key bindings to use in the program. */
    KeyBindings bindings;
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
    KeyBindings parse_bindings
      (const toml::node_view<toml::node> &keys_node);
    /** @brief Parse a key. */
    char parse_key (const toml::node_view<toml::node> &key_node);
  };
} // namespace config

#endif // CONFIG_H
