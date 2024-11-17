#include "ui/keys/bound_keys.h"
#include "cli.h"
#include "csv_exporter.h"
#include "db/db_sqlite.h"
#include "time_lib/duration_displayer.h"
#include "log_lib/logger.h"
#include "time_lib/time_zone.h"
#include "ui.h"
#include "config.h"
#include "version/version.h"

int main(int argc, const char *const *argv) {
  // CLI
  auto cli_data = cli::parse(argc, argv);

  // Loading the configuration file.
  auto config = (cli_data.config_filepath.empty()) ?
    config::ConfigLoader().load() :
    config::ConfigLoader().load(cli_data.config_filepath);

  // Initialize the TimeZone.
  time_lib::TimeZone::get(config.timezone);

  // Initialize the Logger.
  log_lib::Logger::get(config.log_filepath, config.log_levels_to_activate);
  log_lib::logger().tick(); // For measuring startup time.

  log_lib::Logger::get().
    log("Launching timesheeting v" + version::TIMESHEETING_VERSION,
        log_lib::LogLevel::info);

  // Initialize the DB.
  db::DB_SQLite::get(config.db_filepath);

  // Initialize the key bindings.
  keys::BoundKeys::get(&config.bindings);

  // Export to csv
  if (not(cli_data.beg_date.empty()) and not(cli_data.end_date.empty())
      and not(cli_data.export_file.empty())) {
    CSVExporter(cli_data.beg_date, cli_data.end_date,
                cli_data.export_file);
    return 0; // Terminate the program after CSV export.
  }

  // initialize the DurationDisplayer
  time_lib::DurationDisplayer::get(config.hours_per_workday);

  // Loading the UI and entering input loop.
  UI ui;
  ui.input_loop();

  log_lib::logger().tock(); // For measuring teardown time.
}
