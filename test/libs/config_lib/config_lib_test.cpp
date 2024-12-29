#include "config_lib_test.h"
#include <cstdlib>
#include <filesystem>

TEST_CASE("LT-CON-010 Tilde expansion nominal", "[config_lib]") {
  const std::filesystem::path in_path{
      "~/.config/timesheeting/timesheeting.toml"};
  std::filesystem::path ref_path =
    std::getenv("HOME");
  ref_path /= std::filesystem::path{".config/timesheeting/timesheeting.toml"};
  const auto out_path = config_lib::expand_tilde(in_path);
  REQUIRE(out_path == ref_path);
}

TEST_CASE("LT-CON-020 Tilde expansion none", "[config_lib]") {
  const std::filesystem::path in_path{"/etc/timesheeting/timesheeting.toml"};
  const auto out_path = config_lib::expand_tilde(in_path);
  REQUIRE(out_path == in_path);
}
