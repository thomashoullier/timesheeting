/** @file
 *  @brief ExportRow definition. */
#ifndef EXPORT_ROW_H
#define EXPORT_ROW_H

#include "id.h"
#include "time_lib/date.h"
#include <string>

namespace core {
  /** @brief Struct for holding an individual row of the DB export.
             A single row corresponds to a single timesheet entry. */
  struct ExportRow {
    /** @brief The Id of the entry. */
    Id entry_id;
    /** @brief The Id of the project associated with the entry. */
    Id project_id;
    /** @brief The name of the project associated with the entry. */
    std::string project_name;
    /** @brief The Id of the task associated with the entry. */
    Id task_id;
    /** @brief The name of the task associated with the entry. */
    std::string task_name;
    /** @brief The Id of the location associated with the entry. */
    Id location_id;
    /** @brief The name of the location associated with the entry. */
    std::string location_name;
    /** @brief The start date of the entry. */
    time_lib::Date start_date;
    /** @brief The stop date of the entry. */
    time_lib::Date stop_date;

    /** @brief Export the row to a CSV format. */
    std::string to_csv () const;
  };
}
#endif // EXPORT_ROW_H
