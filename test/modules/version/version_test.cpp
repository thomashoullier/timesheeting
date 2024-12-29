#include "version_test.h"
#include "version/version.h"
#include <regex>
#include <string>

TEST_CASE("MT-VER-010 Program version number", "[version]") {
  const std::string program_version = version::TIMESHEETING_VERSION;
  std::string regex_str = "[0123456789]+\\.[0123456789]+(?:dev)?";
  std::regex regex(regex_str);
  REQUIRE(std::regex_match(program_version, regex));
}

TEST_CASE("MT-VER-020 DB version number", "[version]") {
  const int db_version = version::TIMESHEETING_DB_VERSION;
  REQUIRE(db_version > 0);
}
