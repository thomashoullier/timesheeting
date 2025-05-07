#include "exporter_test.h"
#include "core/export_row.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "time_lib/time_zone.h"
#include "test_utils/test_utils.h"
#include "exporter/csv_exporter.h"
#include <catch2/catch_test_macros.hpp>
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
}
