#ifndef CONFIG_TEST_H
#define CONFIG_TEST_H

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

/** @brief Generate the example timesheeting configuration file in the
           given directory. Return the filepath. */
std::filesystem::path generate_example_config
    (const std::filesystem::path &dirpath);

#endif // CONFIG_TEST_H
