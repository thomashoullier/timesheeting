#include "test_utils.h"
#include <filesystem>
#include <random>
#include <stdexcept>
#include <string>

namespace test_utils {

  // TODO: make a manager singleton which cleans up the created folders/files.

  std::filesystem::path get_new_tempdir(const std::string &prefix) {
    auto parent = std::filesystem::temp_directory_path();
    std::string dirname = prefix + random_string(6);
    std::filesystem::path dirpath = parent / dirname;
    if (std::filesystem::exists(dirpath)) {
      throw std::runtime_error("Temporary directory " + dirpath.string() +
                               " already exists!");
    }
    std::filesystem::create_directory(dirpath);
    std::filesystem::permissions(dirpath,
                                 std::filesystem::perms::owner_all);
    return dirpath;
  }

  std::string random_string(std::size_t length) {
    static const char* chars =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int nchars = 62;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand(0, nchars - 1);
    std::string str = "";
    for (std::size_t i = 0 ; i < length ; ++i) {
      char c = chars[rand(rng)];
      str += c;
    }
    return str;
  }

} // namespace test_utils
