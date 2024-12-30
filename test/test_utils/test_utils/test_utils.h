#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <filesystem>

namespace test_utils {
  /** @brief Temporary directory to work with in tests.

   Auto-removed when going out of scope. */
  class TempDir {
  public:
    /** @brief Path to the created directory. */
    std::filesystem::path dirpath;
    /** @brief Return a TempDir named with the given prefix. */
    TempDir(const std::string &prefix);
    /** @brief Auto-delete the directory. */
    ~TempDir();
  };

  /** @brief Get a new temporary directory prefixed by the given name. */
  std::filesystem::path get_new_tempdir (const std::string &prefix);

  /** @brief Get a random string with portable characters of length n. */
  std::string random_string (std::size_t length);
  /** @brief Create a directory with user permissions. */
  void create_user_directory(const std::filesystem::path &dirpath);
  /** @brief Create an empty file. */
  void touch_file (const std::filesystem::path &filepath);

}

#endif // TEST_UTILS_H
