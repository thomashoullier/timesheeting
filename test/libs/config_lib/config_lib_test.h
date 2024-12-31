#ifndef CONFIG_LIB_TEST_H
#define CONFIG_LIB_TEST_H

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

/** @brief Generate the test toml configuration file in the given
    directory. Return the filepath. */
std::filesystem::path generate_test_toml(const std::filesystem::path &dirpath);
/** @brief Generate a test toml configuration file with an empty parameter. */
std::filesystem::path generate_test_toml_empty
    (const std::filesystem::path &dirpath);

#endif // CONFIG_LIB_TEST_H
