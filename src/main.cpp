#include "logger/logger.h"
#include "ui.h"
#include "config.h"

int main() {

  // Loading the configuration file.
  auto config = config::ConfigLoader().load();

  // Initialize the Logger.
  Logger::get(config.log_filepath,
              config.log_levels_to_activate);
  logger().tick(); // For measuring startup time.

  // Loading the UI and entering input loop.
  UI ui;
  ui.input_loop();

  logger().tock(); // For measuring teardown time.
}
