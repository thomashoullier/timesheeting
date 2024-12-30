#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <filesystem>

namespace test_utils {
  /** @brief Get a new temporary directory prefixed by the given name. */
  std::filesystem::path get_new_tempdir (const std::string &prefix);

  /** @brief Get a random string with portable characters of length n. */
  std::string random_string (std::size_t length);

}

#endif // TEST_UTILS_H
