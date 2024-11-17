#ifndef EXPORT_ROW_H
#define EXPORT_ROW_H

#include "core/id.h"
#include "time_lib/date.h"
#include <string>

/** @brief Struct for holding a row in the entry export registry. */
struct ExportRow {
  core::Id entry_id;
  core::Id project_id;
  std::string project_name;
  core::Id task_id;
  std::string task_name;
  core::Id location_id;
  std::string location_name;
  time_lib::Date start_date;
  time_lib::Date stop_date;

  /** @brief Export the row to a CSV format. */
  std::string to_csv () const;
};

#endif // EXPORT_ROW_H
