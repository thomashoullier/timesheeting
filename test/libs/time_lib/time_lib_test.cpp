#include "time_lib_test.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "time_lib/day.h"
#include "time_lib/duration.h"
#include "time_lib/duration_displayer.h"
#include "time_lib/week.h"
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
    auto day = time_lib::Day{ymd};
    day.next();
    SUCCEED("next() called without error");
    auto day_str = day.to_string();
    CHECK(day_str == "22Jan2025 Wed");
  }
  SECTION("LT-DAY-070 Previous") {
    auto day = time_lib::Day{ymd};
    day.previous();
    SUCCEED("previous() called without error");
    auto day_str = day.to_string();
    CHECK(day_str == "20Jan2025 Mon");
  }
}

TEST_CASE("Week", "[time_lib]") {
  const time_lib::Date date = time_lib::Date{"24Jan2025 18:08:30"};
  SECTION("LT-WEK-010 Date initialization") {
    time_lib::Week{date};
    SUCCEED("Week initialized from Date without error.");
  }
  SECTION("LT-WEK-020 DateRange conversion") {
    auto week = time_lib::Week{date};
    auto dr = week.to_date_range();
    SUCCEED("DateRange returned without error.");
    auto start_str = dr.start.to_string();
    CHECK(start_str == "20Jan2025 00:00:00");
    auto stop_str = dr.stop.to_string();
    CHECK(stop_str == "27Jan2025 00:00:00");
  }
  SECTION("LT-WEK-030 Now initialization") {
    time_lib::Week{};
    SUCCEED("Week initiliazed to now without error.");
  }
  SECTION("LT-WEK-040 String representation") {
    auto week = time_lib::Week{date};
    auto str = week.to_string();
    CHECK(str == "20Jan2025 W04");
  }
  SECTION("LT-WEK-050 Array of days") {
    auto week = time_lib::Week{date};
    auto array = week.days();
    auto mon = array.at(0).to_string();
    CHECK(mon == "20Jan2025 Mon");
    auto tue = array.at(1).to_string();
    CHECK(tue == "21Jan2025 Tue");
    auto wed = array.at(2).to_string();
    CHECK(wed == "22Jan2025 Wed");
    auto thu = array.at(3).to_string();
    CHECK(thu == "23Jan2025 Thu");
    auto fri = array.at(4).to_string();
    CHECK(fri == "24Jan2025 Fri");
    auto sat = array.at(5).to_string();
    CHECK(sat == "25Jan2025 Sat");
    auto sun = array.at(6).to_string();
    CHECK(sun == "26Jan2025 Sun");
  }
  SECTION("LT-WEK-060 Next") {
    auto week = time_lib::Week{date};
    week.next();
    auto str = week.to_string();
    CHECK(str == "27Jan2025 W05");
  }
  SECTION("LT-WEK-070 Previous") {
    auto week = time_lib::Week{date};
    week.previous();
    auto str = week.to_string();
    CHECK(str == "13Jan2025 W03");
  }
}

TEST_CASE("Duration", "[time_lib]") {
  uint64_t secs {104592};
  SECTION("LT-DUR-010 Zero initialization pass") {
    time_lib::Duration{};
    SUCCEED("Duration default initialized without error.");
  }
  SECTION("LT-DUR-020 Zero initialization seconds string") {
    auto dur = time_lib::Duration{};
    auto str = dur.to_second_string();
    CHECK(str == "0 seconds");
  }
  SECTION("LT-DUR-030 Seconds initialization pass") {
    time_lib::Duration{secs};
    SUCCEED("Duration initialized from seconds count without error.");
  }
  SECTION("LT-DUR-040 Seconds initialization value") {
    auto dur = time_lib::Duration{secs};
    auto str = dur.to_second_string();
    CHECK(str == "104592 seconds");
  }
  SECTION("LT-DUR-050 Duration resolution") {
    auto dur1 = time_lib::Duration{104592};
    auto str1 = dur1.to_second_string();
    CHECK(str1 == "104592 seconds");
    auto dur2 = time_lib::Duration{104593};
    auto str2 = dur2.to_second_string();
    CHECK(str2 == "104593 seconds");
  }
  SECTION("LT-DUR-060 Hours string") {
    auto dur = time_lib::Duration{secs};
    auto str = dur.to_hour_string();
    CHECK(str == "29.053 hours");
  }
  SECTION("LT-DUR-070 Days string") {
    auto dur = time_lib::Duration{secs};
    auto str = dur.to_day_string(7.7);
    CHECK(str == "3.773 days");
  }
  SECTION("LT-DUR-080 Seconds short string") {
    auto dur = time_lib::Duration{secs};
    auto str = dur.to_second_shortstring();
    CHECK(str == "104592");
  }
  SECTION("LT-DUR-090 Hours short string") {
    auto dur = time_lib::Duration{secs};
    auto str = dur.to_hour_shortstring();
    CHECK(str == "29.053");
  }
  SECTION("LT-DUR-100 Days short string") {
    auto dur = time_lib::Duration{secs};
    auto str = dur.to_day_shortstring(7.7);
    CHECK(str == "3.773");
  }
  SECTION("LT-DUR-110 Zero second short string") {
    auto dur = time_lib::Duration{};
    auto str = dur.to_second_shortstring();
    CHECK (str == " ");
  }
  SECTION("LT-DUR-120 Zero hour short string") {
    auto dur = time_lib::Duration{};
    auto str = dur.to_hour_shortstring();
    CHECK (str == " ");
  }
  SECTION("LT-DUR-130 Zero day short string") {
    auto dur = time_lib::Duration{};
    auto str = dur.to_day_shortstring(7.7);
    CHECK (str == " ");
  }
  SECTION("LT-DUR-140 Day string invalid hours") {
    auto dur = time_lib::Duration{secs};
    CHECK_THROWS(dur.to_day_string(-3));
  }
  SECTION("LT-DUR-150 Day short string invalid hours") {
    auto dur = time_lib::Duration{secs};
    CHECK_THROWS(dur.to_day_shortstring(-3));
  }
}

TEST_CASE("DurationDisplayer", "[time_lib]") {
  auto dur = time_lib::Duration{104592};
  SECTION("LT-DRD-010 DurationDisplayer initialization pass") {
    time_lib::DurationDisplayer::get(7.7);
    SUCCEED("DurationDisplayer initialized without error.");
  }
  SECTION("LT-DRD-020 Hours string display") {
    auto str_ref = dur.to_hour_string();
    auto str = time_lib::DurationDisplayer::get().to_string(dur);
    CHECK(str == str_ref);
  }
  SECTION("LT-DRD-030 Hours short string display") {
    auto str_ref = dur.to_hour_shortstring();
    auto str = time_lib::DurationDisplayer::get().to_shortstring(dur);
    CHECK(str == str_ref);
  }
  SECTION("LT-DRD-040 Days string display") {
    auto str_ref = dur.to_day_string(7.7);
    time_lib::DurationDisplayer::get().cycle_format();
    SUCCEED("DurationDisplayer format cycled without error.");
    auto str = time_lib::DurationDisplayer::get().to_string(dur);
    CHECK(str == str_ref);
  }
  SECTION("LT-DRD-050 Days short string display") {
    auto str_ref = dur.to_day_shortstring(7.7);
    auto str = time_lib::DurationDisplayer::get().to_shortstring(dur);
    CHECK(str == str_ref);
  }
  SECTION("LT-DRD-060 Seconds string display") {
    auto str_ref = dur.to_second_string();
    time_lib::DurationDisplayer::get().cycle_format();
    SUCCEED("DurationDisplayer format cycled without error.");
    auto str = time_lib::DurationDisplayer::get().to_string(dur);
    CHECK(str == str_ref);
  }
  SECTION("LT-DRD-070 Seconds short string display") {
    auto str_ref = dur.to_second_shortstring();
    auto str = time_lib::DurationDisplayer::get().to_shortstring(dur);
    CHECK(str == str_ref);
  }
  SECTION("LT-DRD-080 Cycling back to hours") {
    auto str_ref = dur.to_hour_string();
    time_lib::DurationDisplayer::get().cycle_format();
    SUCCEED("DurationDisplayer format cycled without error.");
    auto str = time_lib::DurationDisplayer::get().to_string(dur);
    CHECK(str == str_ref);
  }
}
