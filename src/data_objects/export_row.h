#ifndef EXPORT_ROW_H
#define EXPORT_ROW_H

#include "id.h"
#include "time_lib/date.h"
#include <string>

/** @brief Struct for holding a row in the entry export registry. */
struct ExportRow {
  Id entry_id;
  Id project_id;
  std::string project_name;
  Id task_id;
  std::string task_name;
  Id location_id;
  std::string location_name;
  time_lib::Date start_date;
  time_lib::Date stop_date;

  /** @brief Export the row to a CSV format. */
  std::string to_csv () const;
};

#endif // EXPORT_ROW_H
