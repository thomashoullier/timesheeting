#include "time_lib_test.h"
#include "time_lib/date.h"
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
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

TEST_CASE("Date", "[time_lib]") {
  SECTION("LT-DAT-010 std::time_point initialization") {
    auto tp_now = std::chrono::system_clock::now();
    auto tp_init = std::chrono::floor<std::chrono::seconds>(tp_now);
    time_lib::Date{tp_init};
    SUCCEED("Date initialized without error from std::chrono::time_point");
  }
  SECTION("LT-DAT-020 Date std::time_point access") {
    auto tp_now = std::chrono::system_clock::now();
    auto tp_init = std::chrono::floor<std::chrono::seconds>(tp_now);
    auto date = time_lib::Date{tp_init};
    CHECK(date.tp == tp_init);
  }
  SECTION("LT-DAT-030 Current time initialization") {
    auto tp_now = std::chrono::system_clock::now();
    auto tp_ref = std::chrono::floor<std::chrono::seconds>(tp_now);
    auto date = time_lib::Date{};
    auto diff =
      std::chrono::duration_cast<std::chrono::seconds>(date.tp - tp_ref);
    CHECK(diff.count() < 1);
  }
  SECTION("LT-DAT-040 UNIX timestamp initialization") {
    uint64_t timestamp {1737283885};
    time_lib::Date{timestamp};
    SUCCEED("Date initialized with UNIX timestamp without error.");
  }
  SECTION("LT-DAT-050 UNIX timestamp output") {
    uint64_t timestamp{1737283885};
    auto date = time_lib::Date{timestamp};
    uint64_t queried_timestamp = date.to_unix_timestamp();
    CHECK(queried_timestamp == timestamp);
  }
  SECTION("LT-DAT-060 Date string initialization") {
    std::string date_str = "19Jan2025 11:51:25";
    auto date = time_lib::Date{date_str};
    SUCCEED("Date initialized from date string without error.");
    auto timestamp = date.to_unix_timestamp();
    uint64_t ref_timestamp {1737283885};
    CHECK(timestamp == ref_timestamp);
  }
  SECTION("LT-DAT-070 Date string shortcuts 1") {
    std::string date_str = "19Jan2025 11:51";
    auto date = time_lib::Date{date_str};
    SUCCEED("Date initialized from date string without error.");
    auto timestamp = date.to_unix_timestamp();
    uint64_t ref_timestamp {1737283860};
    CHECK(timestamp == ref_timestamp);
  }
  SECTION("LT-DAT-080 Date string shortcuts 2") {
    std::string date_str = "19Jan2025 11";
    auto date = time_lib::Date{date_str};
    SUCCEED("Date initialized from date string without error.");
    auto timestamp = date.to_unix_timestamp();
    uint64_t ref_timestamp {1737280800};
    CHECK(timestamp == ref_timestamp);
  }
  SECTION("LT-DAT-090 Date string shortcuts 3") {
    std::string date_str = "19Jan2025";
    auto date = time_lib::Date{date_str};
    SUCCEED("Date initialized from date string without error.");
    auto timestamp = date.to_unix_timestamp();
    uint64_t ref_timestamp {1737241200};
    CHECK(timestamp == ref_timestamp);
  }
  SECTION("LT-DAT-100 Date string invalid") {
    std::string date_str = "GOOFY";
    CHECK_THROWS(time_lib::Date{date_str});
  }
  SECTION("LT-DAT-110 Date output string") {
    uint64_t timestamp {1737283885};
    auto date = time_lib::Date{timestamp};
    auto date_str = date.to_string();
    CHECK(date_str == "19Jan2025 11:51:25");
  }
  SECTION("LT-DAT-120 Date output hours/minutes") {
    uint64_t timestamp{1737283885};
    auto date = time_lib::Date{timestamp};
    auto date_str = date.to_shortstring();
    CHECK(date_str == "11:51");
  }
  SECTION("LT-DAT-130 Date output unambiguous string") {
    uint64_t timestamp{1737283885};
    auto date = time_lib::Date{timestamp};
    auto date_str = date.to_fullstring();
    CHECK(date_str == "19Jan2025 11:51:25 +0100");
  }
  SECTION("LT-DAT-140 Date output day/month/year") {
    uint64_t timestamp{1737283885};
    auto date = time_lib::Date{timestamp};
    auto date_str = date.get_day_string();
    CHECK(date_str == "19Jan2025");
  }
  SECTION("LT-DAT-150 Beginning of year") {
    std::chrono::zoned_time current{time_lib::TimeZone::get().zone,
                                    std::chrono::system_clock::now()};
    std::string current_year = std::format("{:%Y}", current);
    std::string ref_ymd = "01Jan" + current_year;
    auto year_begin_date = time_lib::Date(time_lib::DatePoint::YearBegin);
    std::string ymd = year_begin_date.get_day_string();
    CHECK(ymd == ref_ymd);
  }
  SECTION("LT-DAT-160 Date comparison") {
    uint64_t tmstp1{1737283885};
    uint64_t tmstp2{1737283886};
    auto date1 = time_lib::Date{tmstp1};
    auto date2 = time_lib::Date{tmstp2};
    CHECK(date1 < date2);
    CHECK(date2 > date1);
  }
}
