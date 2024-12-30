#include "test_utils.h"
#include <filesystem>
#include <fstream>
#include <random>
#include <stdexcept>
#include <string>

namespace test_utils {

  TempDir::TempDir(const std::string &prefix) :
    dirpath {get_new_tempdir(prefix)} {}

  TempDir::~TempDir() {
    std::filesystem::remove_all(dirpath);
  }

  std::filesystem::path get_new_tempdir(const std::string &prefix) {
    auto parent = std::filesystem::temp_directory_path();
    std::string dirname = prefix + random_string(6);
    std::filesystem::path dirpath = parent / dirname;
    create_user_directory(dirpath);
    return dirpath;
  }

  std::string random_string(std::size_t length) {
    static const char *chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int nchars = 62;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand(0,
                                                                  nchars - 1);
    std::string str = "";
    for (std::size_t i = 0; i < length; ++i) {
      char c = chars[rand(rng)];
      str += c;
    }
    return str;
  }

  void create_user_directory(const std::filesystem::path &dirpath) {
    if (std::filesystem::exists(dirpath)) {
      throw std::runtime_error("Directory " + dirpath.string() +
                               " already exists!");
    }
    std::filesystem::create_directory(dirpath);
    std::filesystem::permissions(dirpath, std::filesystem::perms::owner_all);
  }

  void touch_file(const std::filesystem::path &filepath) {
    if (std::filesystem::exists(filepath)) {
      throw std::runtime_error("File " + filepath.string() +
                               " already exists!");
    }
    std::ofstream{filepath};
  }

} // namespace test_utils
