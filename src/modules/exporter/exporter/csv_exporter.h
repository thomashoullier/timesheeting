/** @file
    @brief Exporter system. */
#ifndef CSV_EXPORTER_H
#define CSV_EXPORTER_H

#include "core/export_row.h"
#include "time_lib/date_range.h"
#include <generator>
#include <filesystem>

namespace exporter {
  /** @brief Exporter of DB data to a CSV file. */
  class CSVExporter {
  public:
    /** @brief Export DB entries data for the given time period to a CSV file. */
    explicit CSVExporter(const time_lib::DateRange &date_range,
                         std::generator<core::ExportRow> &rows,
                         const std::filesystem::path &export_filepath);
  };
} // namespace exporter

#endif // CSV_EXPORTER_H
