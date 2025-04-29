#include "ui/keys/bound_keys.h"
#include "ui/cli/cli.h"
#include "exporter/csv_exporter.h"
#include "db/db_sqlite.h"
#include "time_lib/duration_displayer.h"
#include "log_lib/logger.h"
#include "time_lib/time_zone.h"
#include "ui/tui/ui.h"
#include "config/config.h"
#include "version/version.h"
#include <exception>
#include <iostream>
#include <filesystem>
#include <string>

int main(int argc, const char *const *argv) {
  try {
    // CLI
    auto cli_data = cli::parse(argc, argv);

    // Loading the configuration file.
    auto config = (cli_data.config_filepath.empty()) ?
      config::ConfigLoader().load() :
      config::ConfigLoader().load(cli_data.config_filepath);

    // Initialize the TimeZone.
    time_lib::TimeZone::get(config.timezone);

    // Initialize the Logger.
    log_lib::Logger::get(config.log_filepath, config.log_levels_to_activate,
                         config.max_log_age);
    log_lib::logger().tick(); // For measuring startup time.

    log_lib::Logger::get().
      log("Launching timesheeting v" + version::TIMESHEETING_VERSION,
          log_lib::LogLevel::info);

    // Initialize the DB.
    db::DB_SQLite::get(config.db_filepath);
    log_lib::logger().log("Loaded DB v" +
                          std::to_string(db::db().get_user_version()),
                          log_lib::LogLevel::info);

    // Initialize the key bindings.
    keys::BoundKeys::get(&config.bindings);

    // Export to csv
    if (not(cli_data.beg_date.empty()) and not(cli_data.end_date.empty())
        and not(cli_data.export_file.empty())) {
      log_lib::logger().tick();
      exporter::CSVExporter(cli_data.beg_date, cli_data.end_date,
                            cli_data.export_file);
      log_lib::logger().tock();
      return 0; // Terminate the program after CSV export.
    }

    // initialize the DurationDisplayer
    time_lib::DurationDisplayer::get(config.hours_per_workday);

    // Loading the UI and entering input loop.
    tui::UI tui;
    tui.input_loop();

    log_lib::logger().tock(); // For measuring teardown time.
  } catch (std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return 1;
  }
}
