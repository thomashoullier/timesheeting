/** @brief TempDir definition. */
#ifndef TEMP_DIR_H
#define TEMP_DIR_H

#include <filesystem>
#include <cstddef>
#include <string>

namespace log_lib {
  /** @brief Temporary directory for Logger file rotation.

      Auto-removed when going out of scope. */
  class TempDir {
  public:
    /** @brief Path to the created directory*/
    std::filesystem::path dirpath;
    /** @brief Return a TempDir named with the given prefix. */
    explicit TempDir(const std::string &prefix);
    /** @brief Auto-delete the directory. */
    ~TempDir();

  private:
    /** @brief Get a new temporary directory prefixed by the given name. */
    std::filesystem::path get_new_tempdir (const std::string &prefix);

    /** @brief Get a random string with portable characters of length n. */
    std::string random_string (std::size_t length);
    /** @brief Create a directory with user permissions. */
    void create_user_directory(const std::filesystem::path &dirpath);
  };
} // namespace log_lib

#endif // TEMP_DIR_H
