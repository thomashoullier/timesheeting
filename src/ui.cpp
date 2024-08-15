#include "ui.h"

UI::UI()
  : ncurses_handle(),
    db(std::make_shared<DB_SQLite>("timesheeting.db")),
    status_bar(std::make_shared<StatusBarNCurses>()),
    projects_table(db, status_bar),
    logger(&LoggerFile::get()) {
  logger->log("timesheeting UI initialized.");
}

UI::~UI() {}

char UI::input_loop() {
  return projects_table.input_loop();
}
