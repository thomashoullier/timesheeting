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
  UIScreen *cur_screen {&projects_table};
  projects_table.clear();
  entries_table.clear();
  while (true) {
    cur_screen->refresh();
    auto ch = cur_screen->input_loop();
    cur_screen->clear();
    switch (ch) {
    case '1':
      cur_screen = &entries_table;
      break;
    case '2':
      cur_screen = &projects_table;
      break;
    default:
      return ch;
    }
  }
}
