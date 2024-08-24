#include "logger/logger_file.h"
#include "ui.h"

int main() {
  LoggerFile *logger = &LoggerFile::get();
  logger->tick(); // For measuring startup time.

  UI ui;
  ui.input_loop();

  logger->tock(); // For measuring teardown time.
}
