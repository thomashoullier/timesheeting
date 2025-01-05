/** @file
 *  @brief Managing the configuration loading. */
#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <string>
#include <vector>
#include "config_lib/toml_loader.h"
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
    /** @brief Parse key bindings. */
    KeyBindings parse_bindings
    (std::shared_ptr<config_lib::TomlLoader> config_loader);
    /** @brief Set a key (primary and alternate bindings). */
    void set_key(Key &k,
                 std::shared_ptr<config_lib::TomlLoader> config_loader,
                 std::vector<std::string> tree_pos);
    /** @brief Parse a string into a special key binding. */
    int parse_special_string(const std::string &str);
    /** @brief Parse a string into a character key binding. */
    int parse_regular_string(const std::string &str);
    /** @brief Parse a string given for a key binding to a character. */
    int parse_binding_string(const std::string &str);
  };
} // namespace config

#endif // CONFIG_H
