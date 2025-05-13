#include "exporter_test.h"
#include "core/export_row.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "time_lib/time_zone.h"
#include "test_utils/test_utils.h"
#include "exporter/csv_exporter.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <filesystem>
#include <ios>
#include <sstream>
#include <vector>

std::generator<core::ExportRow>
exporter_vec_to_generator(const std::vector<core::ExportRow> &rows_vec) {
  for (const auto &row : rows_vec) { co_yield row; }
}

TEST_CASE("Exporter module") {
  time_lib::TimeZone::get("Europe/Paris");
  time_lib::Date export_start_date
    {time_lib::Date::FullString{}, "01Jan2024 00:00:00 +0100"};
  time_lib::Date export_stop_date
    {time_lib::Date::FullString{}, "31Dec2024 23:59:59 +0100"};
  time_lib::DateRange export_date_range{export_start_date, export_stop_date};
  core::ExportRow row1
    {4, 1, "Project1", 3, "Task3", 1, "Location1",
     time_lib::Date{1729952454}, time_lib::Date{1729953654}};
  core::ExportRow row2
    {8, 1, "Project1", 11, "Task11", 1, "Location1",
     time_lib::Date{1729953659}, time_lib::Date{1729953789}};
  core::ExportRow row3
    {9, 15, "Project15", 5, "Task5", 3, "Location3",
     time_lib::Date{1729953888}, time_lib::Date{1729953988}};
  std::vector<core::ExportRow> rows_vec {row1, row2, row3};
  std::generator<core::ExportRow> rows_gen =
      exporter_vec_to_generator(rows_vec);
  auto exporter_test_folder = test_utils::TempDir("timesheeting_exporter_test");
  auto exporter_test_filepath = exporter_test_folder.dirpath / "export.csv";

  SECTION("MT-EXP-010 Export file directory exception") {
    auto dir_filepath = exporter_test_folder.dirpath;
    CHECK_THROWS(exporter::CSVExporter
                 (export_date_range, rows_gen, dir_filepath));
  }
  SECTION("MT-EXP-020 Export file exists exception") {
    auto existing_filepath = exporter_test_folder.dirpath / "empty.csv";
    {
      std::ofstream out{existing_filepath};
    }
    CHECK_THROWS(exporter::CSVExporter(export_date_range, rows_gen,
                                       existing_filepath));
  }
  SECTION("MT-EXP-030 Export file nonexistent path exception") {
    auto inexistent_export_path =
        exporter_test_folder.dirpath / "nonexistent" / "export.csv";
    CHECK_THROWS(exporter::CSVExporter(export_date_range, rows_gen,
                                       inexistent_export_path));
  }
  SECTION("MT-EXP-040 Export file csv extension exception") {
    auto txt_export_path =
        exporter_test_folder.dirpath / "export.txt";
    CHECK_THROWS(exporter::CSVExporter(export_date_range, rows_gen,
                                       txt_export_path));
  }
  SECTION("Valid case for export file") {
    exporter::CSVExporter(export_date_range, rows_gen,
                          exporter_test_filepath);
    SECTION("MT-EXP-050 Export file") {
      SUCCEED("Export file in valid case created without error.");
      CHECK(std::filesystem::exists(exporter_test_filepath));
      CHECK(std::filesystem::file_size(exporter_test_filepath) > 0);
    }
    SECTION("MT-EXP-060 Export file UTF-8") {
      CHECK(std::filesystem::file_size(exporter_test_filepath) >= 3);
      std::ifstream in(exporter_test_filepath,
                       std::ios_base::in | std::ios_base::binary);
      std::array<char, 3> bytes;
      in.read(bytes.begin(), 3);
      CHECK(bytes.at(0) == 0x23);
      CHECK(bytes.at(1) == 0x20);
      CHECK(bytes.at(2) == 0x45);
    }
    SECTION("MT-EXP-070 Line endings") {
      std::ifstream in(exporter_test_filepath,
                       std::ios_base::in | std::ios_base::binary);
      std::ostringstream ss;
      ss << in.rdbuf();
      CHECK(ss.str().find('\n') != std::string::npos);
      CHECK(ss.str().find('\r') == std::string::npos);
    }
    SECTION("MT-EXP-080 File ending") {
      std::ifstream in(exporter_test_filepath,
                       std::ios_base::in | std::ios_base::binary);
      std::ostringstream ss;
      ss << in.rdbuf();
      std::string file_str = ss.str();
      CHECK(file_str.back() == '\n');
    }
    SECTION("MT-EXP-090 File structure") {
      std::ifstream in(exporter_test_filepath,
                       std::ios_base::in | std::ios_base::binary);
      std::vector<std::string> lines;
      std::string line;
      while (std::getline(in, line)) {
        lines.push_back(line);
      }
      CHECK(lines.size() == 10);
      CHECK(lines.at(0).front() == '#');
      CHECK(lines.at(1).front() == '#');
      CHECK(lines.at(2).front() == '#');
      CHECK(lines.at(3).front() == '#');
      CHECK(lines.at(4).front() == '#');
      CHECK(lines.at(5).front() == '#');
      CHECK(lines.at(6).front() != '#');
      CHECK(lines.at(7).front() != '#');
      CHECK(lines.at(8).front() != '#');
      CHECK(lines.at(9).front() != '#');
    }
  }
}
