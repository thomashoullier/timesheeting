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
};


#endif // CONFIG_H
