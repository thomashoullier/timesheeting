#include "csv_exporter.h"
#include "time_lib/date.h"
#include "time_lib/date_range.h"
#include "db/db_sqlite.h"
#include "time_lib/time_zone.h"
#include "version.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <ostream>
#include <stdexcept>

CSVExporter::CSVExporter(const std::string &beg_date_str,
                         const std::string &end_date_str,
                         const std::filesystem::path &export_filepath) {
  time_lib::Date beg_date(beg_date_str);
  time_lib::Date end_date(end_date_str);

  // Errors
  if (beg_date > end_date)
    throw std::runtime_error("Provided period begin and end dates have "
                             "the wrong order.");
  if (std::filesystem::is_directory(export_filepath))
    throw std::runtime_error("Export path is a directory.");
  if (export_filepath.extension() != ".csv")
    throw std::runtime_error("Export file extension must be '.csv'.");
  if (std::filesystem::exists(export_filepath))
    throw std::runtime_error("Export file already exists.");

  // Open file
  auto export_file = std::make_unique<std::ofstream>(export_filepath);
  if (!export_file->good()) {
    throw std::runtime_error("Export file was not opened correctly.");
  }

  // Write header
  *export_file << "# Export date: " << time_lib::Date().to_string() << std::endl
               << "# Export start date: " << beg_date.to_string() << std::endl
               << "# Export stop date: " << end_date.to_string() << std::endl
               << "# Header timezone: " << time_lib::TimeZone::get().name()
               << std::endl
               << "# timesheeting version: "
               << version::TIMESHEETING_VERSION << std::endl
               << "# timesheeting DB version: "
               << std::to_string(version::TIMESHEETING_DB_VERSION) << std::endl;

  // Write body
  // TODO Call DB line std::generator and write csv body line by line.
  *export_file <<
    "Entry ID, Project ID, Project name, Task ID, Task name, Location ID, "
    "Location name, Start date, Stop date" << std::endl;
  time_lib::DateRange period(beg_date, end_date);
  auto full_register = db().query_export_entries(period);
  for (const auto &row : full_register) {
    *export_file << row.to_csv() << std::endl;
  }
}
