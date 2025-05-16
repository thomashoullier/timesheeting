#include "exporter_test.h"
#include "core/export_row.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "time_lib/time_zone.h"
#include "test_utils/test_utils.h"
#include "exporter/csv_exporter.h"
#include "version/version.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <filesystem>
#include <ios>
#include <regex>
#include <sstream>
#include <string>
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
    SECTION("Lines set") {
      std::vector<std::string> lines;
      {
        std::ifstream in(exporter_test_filepath,
                         std::ios_base::in | std::ios_base::binary);
        std::string line;
        while (std::getline(in, line)) {
          lines.push_back(line);
        }
      }
      SECTION("MT-EXP-090 File structure") {
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
      SECTION("Export date") {
        std::string reference_prefix {"# Export date: "};
        std::string line = lines.at(0);
        SECTION("MT-EXP-100 Export date line") {
          CHECK(line.size() > reference_prefix.size());
          CHECK(line.substr(0, reference_prefix.size()) == reference_prefix);
        }
        SECTION("MT-EXP-110 Export date value") {
          time_lib::Date date_now{};
          std::string date_str =
              line.substr(reference_prefix.size());
          time_lib::Date export_date{time_lib::Date::FullString{}, date_str};
          uint64_t error_tolerance = 60; // seconds
          CHECK((date_now.to_unix_timestamp() -
                 export_date.to_unix_timestamp()) < error_tolerance);
        }
      }
      SECTION("MT-EXP-120 Export date timezone") {
        std::string line = lines.at(0);
        auto line_size = line.size();
        CHECK(line_size >= 5);
        std::string last_five = line.substr(line_size - 5);
        CHECK(last_five.size() == 5);
        std::regex re("^[+-][0-9]{4}$");
        CHECK(std::regex_search(last_five, re));
      }
      SECTION("Period start date") {
        std::string reference_prefix{"# Export start date: "};
        std::string line = lines.at(1);
        SECTION("MT-EXP-130 Period start date format") {
          CHECK(line.size() > reference_prefix.size());
          CHECK(line.substr(0, reference_prefix.size()) == reference_prefix);
        }
        SECTION("MT-EXP-140 Period start date value") {
          std::string after_prefix = line.substr(reference_prefix.size());
          std::string reference_str{"01Jan2024 00:00:00 +0100"};
          CHECK(after_prefix == reference_str);
        }
      }
      SECTION("Period stop date") {
        std::string reference_prefix{"# Export stop date: "};
        std::string line = lines.at(2);
        SECTION("MT-EXP-150 Period stop date format") {
          CHECK(line.size() > reference_prefix.size());
          CHECK(line.substr(0, reference_prefix.size()) == reference_prefix);
        }
        SECTION("MT-EXP-160 Period stop date value") {
          std::string after_prefix = line.substr(reference_prefix.size());
          std::string reference_str{"31Dec2024 23:59:59 +0100"};
          CHECK(after_prefix == reference_str);
        }
      }
      SECTION("Header timezone") {
        std::string reference_prefix{"# Header timezone: "};
        std::string line = lines.at(3);
        SECTION("MT-EXP-170 Header timezone format") {
          CHECK(line.size() > reference_prefix.size());
          CHECK(line.substr(0, reference_prefix.size()) == reference_prefix);
        }
        SECTION("MT-EXP-180 Header timezone value") {
          std::string reference_str{"Europe/Paris"};
          std::string after_prefix = line.substr(reference_prefix.size());
          CHECK(after_prefix == reference_str);
        }
      }
      SECTION("Program version") {
        std::string reference_prefix{"# timesheeting version: "};
        std::string line = lines.at(4);
        SECTION("MT-EXP-190 Program version format") {
          CHECK(line.size() > reference_prefix.size());
          CHECK(line.substr(0, reference_prefix.size()) == reference_prefix);
        }
        SECTION("MT-EXP-200 Program version value") {
          std::string reference_str{version::TIMESHEETING_VERSION};
          std::string after_prefix = line.substr(reference_prefix.size());
          CHECK(after_prefix == reference_str);
        }
      }
      SECTION("DB version") {
        std::string reference_prefix{"# timesheeting DB version: "};
        std::string line = lines.at(5);
        SECTION("MT-EXP-210 DB version format") {
          CHECK(line.size() > reference_prefix.size());
          CHECK(line.substr(0, reference_prefix.size()) == reference_prefix);
        }
        SECTION("MT-EXP-220 DB version value") {
          auto reference_version = version::TIMESHEETING_DB_VERSION;
          std::string reference_str = std::to_string(reference_version);
          std::string after_prefix = line.substr(reference_prefix.size());
          CHECK(after_prefix == reference_str);
        }
      }
      SECTION("MT-EXP-230 CSV format") {
        std::regex re(",\\S");
        for (int iline = 6; iline < 10; ++iline) {
          std::string line = lines.at(iline);
          CHECK(line.find(','));
          CHECK_FALSE(std::regex_search(line, re));
        }
      }
      SECTION("MT-EXP-240 Column list") {
        const std::string reference_string{
            "Entry ID, Project ID, Project name, Task ID, Task name, Location "
            "ID, Location name, Start date, Stop date"};
        const std::string line = lines.at(6);
        CHECK(line == reference_string);
      }
      SECTION("MT-EXP-250 No empty fields") {
        std::regex re(",\\s*,");
        for (int iline = 6; iline < 10; ++iline) {
          std::string line = lines.at(iline);
          CHECK_FALSE(std::regex_search(line, re));
        }
      }
      SECTION("MT-EXP-260 Entries lines") {
        const std::string entry_1{
            "4, 1, Project1, 3, Task3, 1, Location1, 1729952454, 1729953654"};
        CHECK(lines.at(7) == entry_1);
        const std::string entry_2{
            "8, 1, Project1, 11, Task11, 1, Location1, 1729953659, 1729953789"};
        CHECK(lines.at(8) == entry_2);
        const std::string entry_3{
            "9, 15, Project15, 5, Task5, 3, Location3, 1729953888, 1729953988"};
        CHECK(lines.at(9) == entry_3);
      }
    }
  }
}
