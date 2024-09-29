#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>

struct UserConfig {
  std::filesystem::path log_filepath;
};

/** @brief Config file loader class. */
class ConfigLoader {
public:
  /** @brief Load the configuration file into an internal representation. */
  UserConfig load(const std::filesystem::path &config_file);

private:
  /** @brief Check the configuration is valid. */
  void check_configuration ();
  /** @brief Expand tilde and braces/variables in file paths. */
  std::filesystem::path expand_tilde(const std::filesystem::path &path);
};


#endif // CONFIG_H
