/** @file
 *  @brief Configuration file utilities library. */
#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include <filesystem>
#include <optional>
#include <vector>

namespace config_lib {
  /** @brief Expand a tilder in file paths. */
  std::filesystem::path expand_tilde(const std::filesystem::path &path);
  /** @brief Find a given file given a series of folders and a suffix.

      The first found file path with an existing file is returned.
      Nothing is returned if nothing was found. */
  std::optional<std::filesystem::path> find_file
  (const std::vector<std::filesystem::path> &folders,
   const std::filesystem::path &suffix);
}

#endif // CONFIG_UTILS_H
