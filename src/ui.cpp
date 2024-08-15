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
  // TODO: create a Screen virtual class which ProjectTaskTable and EntriesTable
  //       implement, with methods refresh, input_loop, and a clear.
  // TEMP Set the project screen as the first displayed.
  projects_table.refresh();
  auto ch = projects_table.input_loop();
  while (true) {
    switch (ch) {
    case '1':
      projects_table.clear();
      entries_table.refresh();
      ch = entries_table.input_loop();
      break;
    case '2':
      entries_table.clear();
      projects_table.refresh();
      ch = projects_table.input_loop();
      break;
    default:
      return ch;
    }
  }
}
