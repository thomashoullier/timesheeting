#include "csv_exporter.h"
#include "data_objects/date.h"

CSVExporter::CSVExporter(const std::string &beg_date_str,
                         const std::string &end_date_str,
                         const std::filesystem::path &export_filepath) {
  Date beg_date(beg_date_str);
  Date end_date(end_date_str);

  // TODO:
  // 1. Check dates are in order.
  // 2. Check csv extension.
  // 3. Check export file does not already exist.
  // 4. Check export folder exists.
  // 5. Check file can be created/is writeable.
}
