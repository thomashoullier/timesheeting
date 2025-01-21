#include "time_lib_test.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "time_lib/day.h"
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

TEST_CASE("DateRange", "[time_lib]") {
  uint64_t tmstp1{1737283885};
  uint64_t tmstp2{1737283886};
  auto start = time_lib::Date{tmstp1};
  auto stop = time_lib::Date{tmstp2};
  SECTION("LT-DTR-010 DateRange initialization") {
    time_lib::DateRange(start, stop);
    SUCCEED("DateRange initialized without error");
  }
  SECTION("LT-DTR-020 DateRange ordering") {
    CHECK_THROWS(time_lib::DateRange(stop, start));
  }
  SECTION("LT-DTR-030 Dates read access") {
    auto date_range = time_lib::DateRange(start, stop);
    auto start_stamp = date_range.start.to_unix_timestamp();
    auto stop_stamp = date_range.stop.to_unix_timestamp();
    CHECK(start_stamp == tmstp1);
    CHECK(stop_stamp == tmstp2);
  }
  SECTION("LT-DTR-040 DateRange to strings") {
    auto date_range = time_lib::DateRange(start, stop);
    auto dr_strings = date_range.to_string();
    auto ref_start_str = start.to_string();
    auto ref_stop_str = stop.to_string();
    CHECK(dr_strings.at(0) == ref_start_str);
    CHECK(dr_strings.at(1) == ref_stop_str);
  }
  SECTION("LT-DTR-050 DateRange to day strings") {
    auto date_range = time_lib::DateRange(start, stop);
    auto dr_strings = date_range.to_day_strings();
    auto ref_start_str = start.get_day_string();
    auto ref_stop_str = stop.get_day_string();
    CHECK(dr_strings.at(0) == ref_start_str);
    CHECK(dr_strings.at(1) == ref_stop_str);
  }
}

TEST_CASE("Day", "[time_lib]") {
  std::chrono::year y{2025};
  std::chrono::month m{std::chrono::January};
  std::chrono::day d{21};
  std::chrono::year_month_day ymd(y, m, d);
  SECTION("LT-DAY-010 Day ymd initialization") {
    time_lib::Day{ymd};
    SUCCEED("ymd Day initialized without error");
  }
  SECTION("LT-DAY-020 DateRange representation") {
    auto day = time_lib::Day{ymd};
    auto dr = day.to_date_range();
    CHECK(std::is_same<decltype(dr), time_lib::DateRange>());
  }
  SECTION("LT-DAY-030 DateRange start and stop Date") {
    auto day = time_lib::Day{ymd};
    auto dr = day.to_date_range();
    auto start_str = dr.start.to_string();
    CHECK(start_str == "21Jan2025 00:00:00");
    auto stop_str = dr.stop.to_string();
    CHECK(stop_str == "22Jan2025 00:00:00");
  }
  SECTION("LT-DAY-040 String representation") {
    auto day = time_lib::Day{ymd};
    auto str = day.to_string();
    CHECK(str == "21Jan2025 Tue");
  }
  SECTION("LT-DAY-050 Now initialization") {
    auto day = time_lib::Day{};
    auto dr = day.to_date_range();
    auto date_now = time_lib::Date{};
    auto now_ymd = date_now.get_day_string();
    auto ymd_start = now_ymd + " 00:00:00";
    auto dr_start_str = dr.start.to_string();
    CHECK(dr_start_str == ymd_start);
  }
  SECTION("LT-DAY-060 Next") {
    auto day = time_lib::Day{};
    day.next();
    SUCCEED("next() called without error");
    auto day_str = day.to_string();
    CHECK(day_str == "22Jan2025 Wed");
  }
  SECTION("LT-DAY-070 Previous") {
    auto day = time_lib::Day{};
    day.previous();
    SUCCEED("previous() called without error");
    auto day_str = day.to_string();
    CHECK(day_str == "20Jan2025 Mon");
  }
}
