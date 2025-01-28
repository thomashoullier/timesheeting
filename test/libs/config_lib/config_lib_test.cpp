#include "config_lib_test.h"
#include "config_lib/config_utils.h"
#include "config_lib/toml_loader.h"
#include "test_utils/test_utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>

std::filesystem::path generate_test_toml(const std::filesystem::path &dirpath) {
  const std::string content =
    "[par1]\n"
    "string_arg = \"Hello there.\"\n"
    "empty_string_arg = \"\"\n"
    "\n"
    "[par2]\n"
    "  [par2.sub1]\n"
    "  filepath_arg = \"~/\"\n"
    "  filepath_nonexistent_arg = \"/dev/null/nonexistent/nonexistent\"\n"
    "  [par2.sub2]\n"
    "  float_arg = 8.3\n"
    "  unsigned_arg = 1089\n"
    "  [par2.sub3]\n"
    "  vector_strings_arg = [ \"hello\", \"there\", \"fox\", \"delta\" ]\n"
    "  vector_nonstrings_arg = [ 31, 1, 43 ]\n";
  const std::filesystem::path filepath {dirpath / "config_lib_testfile.toml"};
  std::ofstream out {filepath};
  out << content;
  return filepath;
}

std::filesystem::path
generate_test_toml_empty(const std::filesystem::path &dirpath) {
  const std::string content = "[par1]\n"
                              "string_arg = \"Hello there.\"\n"
                              "empty_arg = \n";
  const std::filesystem::path filepath
    {dirpath / "config_lib_testfile_empty.toml"};
  std::ofstream out {filepath};
  out << content;
  return filepath;
}

TEST_CASE("LT-CON-010 Tilde expansion nominal", "[config_lib]") {
  const std::filesystem::path in_path{
      "~/.config/timesheeting/timesheeting.toml"};
  std::filesystem::path ref_path = std::getenv("HOME");
  ref_path /= std::filesystem::path{".config/timesheeting/timesheeting.toml"};
  const auto out_path = config_lib::expand_tilde(in_path);
  CHECK(out_path == ref_path);
}

TEST_CASE("LT-CON-020 Tilde expansion none", "[config_lib]") {
  const std::filesystem::path in_path{"/etc/timesheeting/timesheeting.toml"};
  const auto out_path = config_lib::expand_tilde(in_path);
  CHECK(out_path == in_path);
}

TEST_CASE("First existing file", "[config_lib]") {
  auto dir = test_utils::TempDir("timesheeting_config_lib_test");
  // Create the directory structure
  test_utils::create_user_directory(dir.dirpath / "dir1");
  test_utils::create_user_directory(dir.dirpath / "dir1" / "subdir1");
  test_utils::create_user_directory(dir.dirpath / "dir2");
  test_utils::touch_file(dir.dirpath / "dir2" / "timesheeting.toml");
  test_utils::create_user_directory(dir.dirpath / "dir3");
  SECTION ("LT-CON-030") {
    const std::vector<std::filesystem::path> folders
      {dir.dirpath / "dir1" / "subdir1",
       dir.dirpath / "dir2",
       dir.dirpath / "dir3"};
    const std::filesystem::path suffix {"timesheeting.toml"};
    const std::filesystem::path ref_path
      {dir.dirpath / "dir2" / "timesheeting.toml"};
    const auto found_path = config_lib::find_file (folders, suffix);
    CHECK(found_path.has_value());
    CHECK(std::filesystem::equivalent(found_path.value(), ref_path));
  }
  SECTION("LT-CON-040") {
    const std::vector<std::filesystem::path> folders{
        dir.dirpath / "dir1" / "subdir1", dir.dirpath / "dir1",
        dir.dirpath / "dir3"};
    const std::filesystem::path suffix {"timesheeting.toml"};
    const auto found_path = config_lib::find_file (folders, suffix);
    CHECK(not(found_path.has_value()));
  }
}

TEST_CASE("Configuration loader", "[config_lib]") {
  // Test setup
  auto dir = test_utils::TempDir("timesheeting_config_lib_test");
  auto config_file = generate_test_toml(dir.dirpath);
  SECTION("LT-CON-050") {
    auto conf = config_lib::TomlLoader(config_file);
    SUCCEED("Configuration file loaded.");
  }
  auto conf = config_lib::TomlLoader(config_file);
  SECTION("LT-CON-060") {
    const std::filesystem::path nonexistent_file{"/dev/null/nonexistent"};
    CHECK_THROWS(config_lib::TomlLoader(nonexistent_file));
  }
  SECTION("LT-CON-070") {
    const auto str = conf.parse_string({"par1", "string_arg"});
    CHECK(str == "Hello there.");
  }
  SECTION("LT-CON-080") {
    CHECK_THROWS(conf.parse_string({"par1", "empty_string_arg"}));
  }
  SECTION("LT-CON-090") {
    const std::filesystem::path ref_path {std::getenv("HOME")};
    const auto filepath = conf.parse_filepath({"par2", "sub1", "filepath_arg"});
    CHECK (std::filesystem::equivalent(filepath, ref_path));
  }
  SECTION("LT-CON-100") {
    CHECK_THROWS(conf.parse_filepath
                 ({"par2", "sub1", "filepath_nonexistent_arg"}));
  }
  SECTION("LT-CON-110") {
    auto num = conf.parse_float({"par2", "sub2", "float_arg"});
    CHECK_THAT(num, Catch::Matchers::WithinAbs(8.3, 0.001));
  }
  SECTION("LT-CON-120") {
    auto config_empty = generate_test_toml_empty(dir.dirpath);
    CHECK_THROWS(config_lib::TomlLoader(config_empty));
  }
  SECTION("LT-CON-130") {
    const auto vec_str =
      conf.parse_stringvec({"par2", "sub3", "vector_strings_arg"});
    std::vector<std::string> ref_vec {"hello", "there", "fox", "delta"};
    CHECK(vec_str == ref_vec);
  }
  SECTION("LT-CON-140") {
    CHECK_THROWS
      (conf.parse_stringvec({"par2", "sub3", "vector_nonstrings_arg"}));
  }
  SECTION("LT-CON-150") {
    auto num = conf.parse_unsigned({"par2", "sub2", "unsigned_arg"});
    CHECK(num == 1089);
  }
}
