#include "export_row.h"

namespace core {
  std::string ExportRow::to_csv() const {
    std::string csv_str =
      std::to_string(entry_id) + ", " +
      std::to_string(project_id) + ", " +
      project_name + ", " +
      std::to_string(task_id) + ", " +
      task_name + ", " +
      std::to_string(location_id) + ", " +
      location_name + ", " +
      std::to_string(start_date.to_unix_timestamp()) + ", " +
      std::to_string(stop_date.to_unix_timestamp());
    return csv_str;
  }
}
