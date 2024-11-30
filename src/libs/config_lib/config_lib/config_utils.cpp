#include "config_utils.h"
#include <filesystem>

namespace config_lib {
  std::filesystem::path expand_tilde(const std::filesystem::path &path) {
    std::string str {path};
    if (str.starts_with("~/")) {
      std::filesystem::path home_dir = std::getenv("HOME");
      return home_dir / str.substr(2);
    }
    return path;
  }

  std::optional<std::filesystem::path>
  find_file(const std::vector<std::filesystem::path> &folders,
            const std::filesystem::path &suffix) {
    for (const auto &folder : folders) {
      std::filesystem::path try_path = folder / suffix;
      if (std::filesystem::exists(try_path))
        return try_path;
    }
    return std::nullopt;
  }
} // namespace config_lib
