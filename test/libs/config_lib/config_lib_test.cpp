#include "config_lib_test.h"
#include "config_lib/config_utils.h"
#include "test_utils/test_utils.h"
#include <cstdlib>
#include <filesystem>

TEST_CASE("LT-CON-010 Tilde expansion nominal", "[config_lib]") {
  const std::filesystem::path in_path{
      "~/.config/timesheeting/timesheeting.toml"};
  std::filesystem::path ref_path = std::getenv("HOME");
  ref_path /= std::filesystem::path{".config/timesheeting/timesheeting.toml"};
  const auto out_path = config_lib::expand_tilde(in_path);
  REQUIRE(out_path == ref_path);
}

TEST_CASE("LT-CON-020 Tilde expansion none", "[config_lib]") {
  const std::filesystem::path in_path{"/etc/timesheeting/timesheeting.toml"};
  const auto out_path = config_lib::expand_tilde(in_path);
  REQUIRE(out_path == in_path);
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
    REQUIRE(found_path.has_value());
    REQUIRE(found_path == ref_path);
  }
  SECTION("LT-CON-040") {
    const std::vector<std::filesystem::path> folders{
        dir.dirpath / "dir1" / "subdir1", dir.dirpath / "dir1",
        dir.dirpath / "dir3"};
    const std::filesystem::path suffix {"timesheeting.toml"};
    const auto found_path = config_lib::find_file (folders, suffix);
    REQUIRE(not(found_path.has_value()));
  }
}
