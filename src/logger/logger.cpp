#include "logger.h"
#include "logger_file.h"

void log(const std::string &msg) {
  auto *logger = &LoggerFile::get();
  logger->log(msg);
}

void tick() {
  auto *logger = &LoggerFile::get();
  logger->tick();
}

void tock() {
  auto *logger = &LoggerFile::get();
  logger->tock();
}
