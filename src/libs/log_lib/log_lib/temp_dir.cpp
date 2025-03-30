#include "temp_dir.h"

#include <random>

namespace log_lib {
  TempDir::TempDir(const std::string &prefix) :
    dirpath {get_new_tempdir(prefix)} {}

  TempDir::~TempDir() {
    std::filesystem::remove_all(dirpath);
  }

  std::filesystem::path TempDir::get_new_tempdir(const std::string &prefix) {
    auto parent = std::filesystem::temp_directory_path();
    std::string dirname = prefix + random_string(6);
    std::filesystem::path tempdir = parent / dirname;
    create_user_directory(tempdir);
    return tempdir;
  }

  std::string TempDir::random_string(std::size_t length) {
    static const char *chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int nchars = 62;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
      rand(0, nchars - 1);
    std::string str = "";
    for (std::size_t i = 0; i < length; ++i) {
      char c = chars[rand(rng)];
      str += c;
    }
    return str;
  }

  void TempDir::create_user_directory(const std::filesystem::path &dirpath) {
    if (std::filesystem::exists(dirpath)) {
      throw std::runtime_error("Directory " + dirpath.string() +
                               " already exists!");
    }
    std::filesystem::create_directory(dirpath);
    std::filesystem::permissions(dirpath, std::filesystem::perms::owner_all);
  }
}
