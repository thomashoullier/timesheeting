#include "logger/logger.h"
#include "ui.h"

int main() {
  tick(); // For measuring startup time.

  UI ui;
  ui.input_loop();

  tock(); // For measuring teardown time.
}
