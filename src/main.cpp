#include "cli.h"
#include "db/db_sqlite.h"
#include "duration_displayer.h"
#include "logger/logger.h"
#include "time_zone.h"
#include "ui.h"
#include "config.h"
#include "version.h"

int main(int argc, const char *const *argv) {
  // CLI
  auto cli_data = cli::parse(argc, argv);

  // Loading the configuration file.
  auto config = (cli_data.config_filepath.empty()) ?
    config::ConfigLoader().load() :
    config::ConfigLoader().load(cli_data.config_filepath);

  // Initialize the Logger.
  Logger::get(config.log_filepath,
              config.log_levels_to_activate);
  logger().tick(); // For measuring startup time.

  Logger::get().log("Launching timesheeting v" + version::TIMESHEETING_VERSION,
                    LogLevel::info);

  // Initialize the TimeZone.
  TimeZone::get(config.timezone);

  // Initialize the DB.
  DB_SQLite::get(config.db_filepath);

  // Initialize the DurationDisplayer
  DurationDisplayer::get(config.hours_per_workday);

  // Loading the UI and entering input loop.
  UI ui;
  ui.input_loop();

  logger().tock(); // For measuring teardown time.
}
