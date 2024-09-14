#include "logger/logger.h"
#include "ui.h"

int main() {
  logger().tick(); // For measuring startup time.

  UI ui;
  ui.input_loop();

  logger().tock(); // For measuring teardown time.
}
