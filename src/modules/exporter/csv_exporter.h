#ifndef CSV_EXPORTER_H
#define CSV_EXPORTER_H

#include <string>
#include <filesystem>

namespace exporter {
  class CSVExporter {
  public:
    /** @brief Export DB entries data for the given time period to a CSV file. */
    CSVExporter(const std::string &beg_date_str, const std::string &end_date_str,
                const std::filesystem::path &export_filepath);
  };
} // namespace exporter

#endif // CSV_EXPORTER_H
