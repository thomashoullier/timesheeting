#include "core_test.h"
#include "core/export_row.h"
#include "core/generic_item.h"
#include "core/entry.h"
#include "core/entry_staging.h"
#include "core/project_total.h"
#include "core/weekly_totals.h"
#include "ncurses_lib/menu_ncurses.h"
#include "ncurses_lib/win_ncurses.h"
#include "time_lib/date.h"
#include "time_lib/duration.h"
#include "time_lib/duration_displayer.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <optional>
#include <cmath>
#include <array>

TEST_CASE("MT-COR-010 Project generic item") {
  core::Project project {4, "project1", true};
  SUCCEED("Project generic item instantiated without error.");
  CHECK(project.id == 4);
  CHECK(project.name == "project1");
  CHECK(project.active);
}

TEST_CASE("MT-COR-020 Task generic item") {
  core::Task task{2, "task1", false};
  SUCCEED("Task generic item instantiated without error.");
  CHECK(task.id == 2);
  CHECK(task.name == "task1");
  CHECK_FALSE(task.active);
}

TEST_CASE("MT-COR-030 Location generic item") {
  core::Location location {3, "location1", true};
  SUCCEED("Location generic item instantiated without error.");
  CHECK(location.id == 3);
  CHECK(location.name == "location1");
  CHECK(location.active);
}

TEST_CASE("MT-COR-040 Vector of generic item names") {
  core::Location location1{1, "location1", true};
  core::Location location2{2, "location2", true};
  core::Location location3{3, "location3", true};
  std::vector<core::Location> location_vector {location1, location2, location3};
  std::vector<std::string> name_vector =
    core::generic_item_names(location_vector);
  CHECK(name_vector.size() == 3);
  CHECK(name_vector.at(0) == "location1");
  CHECK(name_vector.at(1) == "location2");
  CHECK(name_vector.at(2) == "location3");
}

TEST_CASE("MT-COR-050 Entry object") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::Entry entry {5, "project1", "task1",
                     start_date, stop_date, "location1"};
  SUCCEED("Entry instantiated without error.");
  CHECK(entry.id == 5);
  CHECK(entry.project_name == "project1");
  CHECK(entry.task_name == "task1");
  CHECK(entry.start.to_unix_timestamp() == start_date.to_unix_timestamp());
  CHECK(entry.stop.to_unix_timestamp() == stop_date.to_unix_timestamp());
  CHECK(entry.location_name == "location1");
}

TEST_CASE("MT-COR-060 Entry to strings") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::Entry entry {5, "project1", "task1",
                     start_date, stop_date, "location1"};
  std::vector<std::string> entry_strings = entry.to_strings();
  CHECK(entry_strings.size() == 5);
  CHECK(entry_strings.at(0) == "project1");
  CHECK(entry_strings.at(1) == "task1");
  CHECK(entry_strings.at(2) == start_date.to_string());
  CHECK(entry_strings.at(3) == stop_date.to_string());
  CHECK(entry_strings.at(4) == "location1");
}

TEST_CASE("MT-COR-070 Entry short to strings") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::Entry entry{5, "project1", "task1", start_date, stop_date, "location1"};
  std::vector<std::string> entry_strings = entry.to_shortstrings();
  CHECK(entry_strings.size() == 5);
  CHECK(entry_strings.at(0) == "project1");
  CHECK(entry_strings.at(1) == "task1");
  CHECK(entry_strings.at(2) == start_date.to_shortstring());
  CHECK(entry_strings.at(3) == stop_date.to_shortstring());
  CHECK(entry_strings.at(4) == "location1");
}

TEST_CASE("MT-COR-080 Entry staging with values") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::EntryStaging entry_staging {"project1", "task1", start_date,
                                    stop_date, "location1"};
  SUCCEED("Entry staging with values instantiated without error.");
  CHECK(entry_staging.project_name.value() == "project1");
  CHECK(entry_staging.task_name.value() == "task1");
  CHECK(entry_staging.start.value().to_unix_timestamp() ==
        start_date.to_unix_timestamp());
  CHECK(entry_staging.stop.value().to_unix_timestamp() ==
        stop_date.to_unix_timestamp());
  CHECK(entry_staging.location_name.value() == "location1");
}

TEST_CASE("MT-COR-090 Entry staging without values") {
  core::EntryStaging entry_staging{std::nullopt, std::nullopt, std::nullopt,
                                   std::nullopt, std::nullopt};
  SUCCEED("Entry staging without values instantiated without error.");
  CHECK_FALSE(entry_staging.project_name.has_value());
  CHECK_FALSE(entry_staging.task_name.has_value());
  CHECK_FALSE(entry_staging.start.has_value());
  CHECK_FALSE(entry_staging.stop.has_value());
  CHECK_FALSE(entry_staging.location_name.has_value());
}

TEST_CASE("MT-COR-100 Entry staging with values to strings") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::EntryStaging entry_staging {"project1", "task1", start_date,
                                    stop_date, "location1"};
  std::vector<std::string> entry_staging_strings = entry_staging.to_strings();
  SUCCEED("Entry staging with values converted "
          "to vector of strings without error.");
  CHECK(entry_staging_strings.size() == 5);
  CHECK(entry_staging_strings.at(0) == "project1");
  CHECK(entry_staging_strings.at(1) == "task1");
  CHECK(entry_staging_strings.at(2) == start_date.to_string());
  CHECK(entry_staging_strings.at(3) == stop_date.to_string());
  CHECK(entry_staging_strings.at(4) == "location1");
}

TEST_CASE("MT-COR-110 Entry staging without values to strings") {
  core::EntryStaging entry_staging{std::nullopt, std::nullopt, std::nullopt,
                                   std::nullopt, std::nullopt};
  std::vector<std::string> entry_staging_strings = entry_staging.to_strings();
  SUCCEED("Entry staging without values converted "
          "to vector of strings without error.");
  CHECK(entry_staging_strings.size() == 5);
  CHECK(entry_staging_strings.at(0) == " ");
  CHECK(entry_staging_strings.at(1) == " ");
  CHECK(entry_staging_strings.at(2) == " ");
  CHECK(entry_staging_strings.at(3) == " ");
  CHECK(entry_staging_strings.at(4) == " ");
}

TEST_CASE("MT-COR-120 Entry staging with values to short strings") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::EntryStaging entry_staging{"project1", "task1", start_date, stop_date,
                                   "location1"};
  std::vector<std::string> entry_staging_strings =
      entry_staging.to_shortstrings();
  SUCCEED("Entry staging with values converted "
          "to vector of short strings without error.");
  CHECK(entry_staging_strings.size() == 5);
  CHECK(entry_staging_strings.at(0) == "project1");
  CHECK(entry_staging_strings.at(1) == "task1");
  CHECK(entry_staging_strings.at(2) == start_date.to_shortstring());
  CHECK(entry_staging_strings.at(3) == stop_date.to_shortstring());
  CHECK(entry_staging_strings.at(4) == "location1");
}

TEST_CASE("MT-COR-130 Entry staging without values to short strings") {
  core::EntryStaging entry_staging{std::nullopt, std::nullopt, std::nullopt,
                                   std::nullopt, std::nullopt};
  std::vector<std::string> entry_staging_strings =
    entry_staging.to_shortstrings();
  SUCCEED("Entry staging without values converted "
          "to vector of short strings without error.");
  CHECK(entry_staging_strings.size() == 5);
  CHECK(entry_staging_strings.at(0) == " ");
  CHECK(entry_staging_strings.at(1) == " ");
  CHECK(entry_staging_strings.at(2) == " ");
  CHECK(entry_staging_strings.at(3) == " ");
  CHECK(entry_staging_strings.at(4) == " ");
}

TEST_CASE("MT-COR-140 Export row") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::ExportRow export_row{4, 15, "project15", 1, "task33",
                             4, "location44", start_date, stop_date};
  SUCCEED("Export row instantiated without error.");
  CHECK(export_row.entry_id == 4);
  CHECK(export_row.project_id == 15);
  CHECK(export_row.project_name == "project15");
  CHECK(export_row.task_id == 1);
  CHECK(export_row.task_name == "task33");
  CHECK(export_row.location_id == 4);
  CHECK(export_row.location_name == "location44");
  CHECK(export_row.start_date.to_unix_timestamp() ==
        start_date.to_unix_timestamp());
  CHECK(export_row.stop_date.to_unix_timestamp() ==
        stop_date.to_unix_timestamp());
}

TEST_CASE("MT-COR-150 Export row csv string") {
  time_lib::Date start_date{1745837288};
  time_lib::Date stop_date{1745838288};
  core::ExportRow export_row {4, 15, "project15", 1, "task33", 4, "location44",
                              start_date, stop_date};
  std::string csv_line = export_row.to_csv();
  SUCCEED("Export row csv line created without error.");
  const std::string ref_line{"4, 15, project15, 1, task33, 4, location44, "
                             "1745837288, 1745838288"};
  CHECK(csv_line == ref_line);
}

TEST_CASE("Project total") {
  time_lib::Duration project1_dur(std::round(13.202 * 3600));
  time_lib::Duration task1_dur(std::round(1.202 * 3600));
  time_lib::Duration task2_dur(std::round(3 * 3600));
  time_lib::Duration task3_dur(std::round(9 * 3600));
  core::TaskTotal task1_total{"Task1", task1_dur};
  core::TaskTotal task2_total{"Task2", task2_dur};
  core::TaskTotal task3_total{"Task3", task3_dur};
  core::ProjectTotal project1_total{
      "Project1", project1_dur, {task1_total, task2_total, task3_total}};
  SECTION("MT-COR-160 Project total instantiation") {
    SUCCEED("ProjectTotal instantiated without error.");
  }
  SECTION("MT-COR-170 Project total to menu items") {
    auto menu_items = project1_total.to_menu_items();
    SUCCEED("Menu items created from project total without error.");
    CHECK(menu_items.size() == 8);
    CHECK(menu_items.at(0).cell_string == "Project1");
    CHECK(menu_items.at(0).display_string == "Project1");
    CHECK(menu_items.at(0).face == ncurses_lib::StringFace::Bold);
    CHECK(menu_items.at(1).cell_string ==
          time_lib::DurationDisplayer::get().to_string(project1_dur));
    CHECK(menu_items.at(1).display_string ==
          time_lib::DurationDisplayer::get().to_string(project1_dur));
    CHECK(menu_items.at(1).face == ncurses_lib::StringFace::Bold);
    CHECK(menu_items.at(2).cell_string == "Task1");
    CHECK(menu_items.at(2).display_string == "Task1");
    CHECK(menu_items.at(2).face == ncurses_lib::StringFace::Normal);
    CHECK(menu_items.at(3).cell_string ==
          time_lib::DurationDisplayer::get().to_string(task1_dur));
    CHECK(menu_items.at(3).display_string ==
          time_lib::DurationDisplayer::get().to_string(task1_dur));
    CHECK(menu_items.at(3).face == ncurses_lib::StringFace::Normal);
    CHECK(menu_items.at(4).cell_string == "Task2");
    CHECK(menu_items.at(4).display_string == "Task2");
    CHECK(menu_items.at(4).face == ncurses_lib::StringFace::Normal);
    CHECK(menu_items.at(5).cell_string ==
          time_lib::DurationDisplayer::get().to_string(task2_dur));
    CHECK(menu_items.at(5).display_string ==
          time_lib::DurationDisplayer::get().to_string(task2_dur));
    CHECK(menu_items.at(5).face == ncurses_lib::StringFace::Normal);
    CHECK(menu_items.at(6).cell_string == "Task3");
    CHECK(menu_items.at(6).display_string == "Task3");
    CHECK(menu_items.at(6).face == ncurses_lib::StringFace::Normal);
    CHECK(menu_items.at(7).cell_string ==
          time_lib::DurationDisplayer::get().to_string(task3_dur));
    CHECK(menu_items.at(7).display_string ==
          time_lib::DurationDisplayer::get().to_string(task3_dur));
    CHECK(menu_items.at(7).face == ncurses_lib::StringFace::Normal);
  }
}

TEST_CASE("Weekly totals") {
  std::array<time_lib::Duration, 7> task1_durs{
      time_lib::Duration(std::round(3.108 * 3600)),
      time_lib::Duration(std::round(1.138 * 3600)),
      time_lib::Duration(std::round(1 * 3600)),
      time_lib::Duration(std::round(2.807 * 3600)),
      time_lib::Duration(std::round(3 * 3600)),
      time_lib::Duration(std::round(2.386 * 3600)),
      time_lib::Duration(std::round(2.039 * 3600))};
  std::array<time_lib::Duration, 7> task2_durs{
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(0.5 * 3600)),
      time_lib::Duration(std::round(0.4 * 3600)),
      time_lib::Duration(std::round(0.5 * 3600)),
      time_lib::Duration(std::round(2 * 3600)),
      time_lib::Duration(std::round(2.5 * 3600)),
      time_lib::Duration(std::round(3 * 3600))};
  std::array<time_lib::Duration, 7> task3_durs{
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(0.5 * 3600)),
      time_lib::Duration(std::round(0.6 * 3600)),
      time_lib::Duration(std::round(1.5 * 3600)),
      time_lib::Duration(std::round(1 * 3600)),
      time_lib::Duration(std::round(2.5 * 3600)),
      time_lib::Duration(std::round(2 * 3600))};
  std::array<time_lib::Duration, 7> task4_durs{
      time_lib::Duration(0),
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(2.114 * 3600)),
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(6.498 * 3600)),
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(5 * 3600))};
  core::PerTaskTotals task1_totals{
      "Task1", time_lib::Duration(std::round(15.478 * 3600)), task1_durs};
  core::PerTaskTotals task2_totals{
      "Task2", time_lib::Duration(std::round(13.9 * 3600)), task2_durs};
  core::PerTaskTotals task3_totals{
      "Task3", time_lib::Duration(std::round(13.1 * 3600)), task3_durs};
  core::PerTaskTotals task4_totals{
      "Task4", time_lib::Duration(std::round(28.612 * 3600)), task4_durs};
  std::array<time_lib::Duration, 7> project1_durs{
      time_lib::Duration(std::round(13.108 * 3600)),
      time_lib::Duration(std::round(2.138 * 3600)),
      time_lib::Duration(std::round(2.000 * 3600)),
      time_lib::Duration(std::round(4.807 * 3600)),
      time_lib::Duration(std::round(6 * 3600)),
      time_lib::Duration(std::round(7.386 * 3600)),
      time_lib::Duration(std::round(7.039 * 3600))};
  std::array<time_lib::Duration, 7> project2_durs{
      time_lib::Duration(0),
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(2.114 * 3600)),
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(6.498 * 3600)),
      time_lib::Duration(std::round(5 * 3600)),
      time_lib::Duration(std::round(5 * 3600))};
  core::PerProjectTotals project1_totals{
      "Project1", time_lib::Duration(std::round(42.478 * 3600)),
      project1_durs, {task1_totals, task2_totals, task3_totals}};
  core::PerProjectTotals project2_totals{
      "Project2",
      time_lib::Duration(std::round(28.612 * 3600)),
      project2_durs,
      {task4_totals}};
  std::array<time_lib::Duration, 7> weekly_durs {
      time_lib::Duration(std::round(13.108 * 3600)),
      time_lib::Duration(std::round(7.138 * 3600)),
      time_lib::Duration(std::round(4.114 * 3600)),
      time_lib::Duration(std::round(9.807 * 3600)),
      time_lib::Duration(std::round(12.498 * 3600)),
      time_lib::Duration(std::round(12.386 * 3600)),
      time_lib::Duration(std::round(12.039 * 3600))};
  core::WeeklyTotals weekly_totals {
    time_lib::Duration(std::round(71.090 * 3600)),
    weekly_durs, {project1_totals, project2_totals}};
  SECTION("MT-COR-180 Weekly totals instantiation") {
    SUCCEED("Weekly totals instantiated without error.");
  }
  SECTION("MT-COR-190 Weekly totals to menu items") {
    auto menu_items = weekly_totals.to_menu_items();
    SUCCEED("Weekly totals converted to menu items without error.");
    CHECK(menu_items->size() == 72);
  }
}
