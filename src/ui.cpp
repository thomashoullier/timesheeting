#include "ui.h"
#include "logger/logger.h"

UI::UI()
  : db(std::make_shared<DB_SQLite>("timesheeting.db")),
    status_bar(std::make_shared<StatusBarNCurses>()),
    projects_screen(db, status_bar),
    locations_screen(db, status_bar),
    entries_screen(db, status_bar) {
  log("timesheeting UI initialized.");
}

UI::~UI() {}

char UI::input_loop() {
  UIComponent *cur_screen {&projects_screen};
  while (true) {
    cur_screen->refresh();
    auto ch = cur_screen->input_loop();
    cur_screen->clear();
    switch (ch) {
    case '1':
      cur_screen = &entries_screen;
      break;
    case '2':
      cur_screen = &projects_screen;
      break;
    case '3':
      cur_screen = &locations_screen;
      break;
    default:
      return ch;
    }
  }
}
