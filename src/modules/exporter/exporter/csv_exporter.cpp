#include "csv_exporter.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "time_lib/time_zone.h"
#include "version/version.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <generator>
#include "core/export_row.h"

namespace exporter {
CSVExporter::CSVExporter(const time_lib::DateRange &date_range,
                         std::generator<core::ExportRow> &rows,
                         const std::filesystem::path &export_filepath) {
  if (std::filesystem::is_directory(export_filepath))
    throw std::runtime_error("Export path is a directory.");
  if (export_filepath.extension() != ".csv")
    throw std::runtime_error("Export file extension must be '.csv'.");
  if (std::filesystem::exists(export_filepath))
    throw std::runtime_error("Export file already exists.");
  auto export_file = std::make_unique<std::ofstream>(export_filepath);
  if (!export_file->good()) {
    throw std::runtime_error("Export file was not opened correctly.");
  }
  write_header(export_file, date_range);
  write_body(export_file, rows);
}

void CSVExporter::write_header(std::unique_ptr<std::ofstream> const &export_file,
                               const time_lib::DateRange &date_range) {
  *export_file << "# Export date: " << time_lib::Date().to_fullstring()
               << std::endl
               << "# Export start date: " << date_range.start.to_fullstring()
               << std::endl
               << "# Export stop date: " << date_range.stop.to_fullstring()
               << std::endl
               << "# Header timezone: " << time_lib::TimeZone::get().name()
               << std::endl
               << "# timesheeting version: " << version::TIMESHEETING_VERSION
               << std::endl
               << "# timesheeting DB version: "
               << std::to_string(version::TIMESHEETING_DB_VERSION) << std::endl;
}

void CSVExporter::write_body(std::unique_ptr<std::ofstream> const &export_file,
                             // cppcheck-suppress constParameterReference
                             std::generator<core::ExportRow> &rows) {
  *export_file
      << "Entry ID, Project ID, Project name, Task ID, Task name, Location ID, "
         "Location name, Start date, Stop date"
      << std::endl;
  for (const auto &row : rows) {
    *export_file << row.to_csv() << std::endl;
  }
}
}
