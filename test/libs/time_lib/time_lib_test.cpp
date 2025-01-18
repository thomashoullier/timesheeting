#include "time_lib_test.h"
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <time_lib/time_zone.h>

TEST_CASE("Time zone singleton", "[time_lib]") {
  SECTION("LT-TMZ-010 Invalid time zone", "[time_zone]") {
    std::string tz_identifier {"GOOFY"};
    CHECK_THROWS(time_lib::TimeZone::get(tz_identifier));
  }
  SECTION("LT-TMZ-010 Time zone initialization", "[time_zone]") {
    std::string tz_identifier {"Europe/Paris"};
    time_lib::TimeZone::get(tz_identifier);
    SUCCEED("TimeZone initialized without error.");
  }
  SECTION("LT-TMZ-030 Time zone name", "[time_zone]") {
    auto tz_name = time_lib::TimeZone::get().name();
    CHECK(tz_name == "Europe/Paris");
  }
  SECTION("LT-TMZ-040 std time_zone", "[time_zone]") {
    auto ref_tz = std::chrono::get_tzdb().locate_zone("Europe/Paris");
    auto tz = time_lib::TimeZone::get().zone;
    CHECK(ref_tz == tz);
  }
}
