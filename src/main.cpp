#include "logger/logger.h"
#include "ui.h"
#include "config.h"

int main() {

  // Loading the configuration file.
  // TODO: use "~" expansion: detect leading '~/' and replace with home.
  auto config =
      ConfigLoader().load("/home/hori/.config/timesheeting/timesheeting.toml");

  // Initialize the Logger.
  Logger::get(config.log_filepath);
  logger().tick(); // For measuring startup time.

  // Loading the UI and entering input loop.
  UI ui;
  ui.input_loop();

  logger().tock(); // For measuring teardown time.
}
