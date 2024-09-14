#include "ui.h"
#include "logger/logger.h"
#include "ui_screens/project_report_screen.h"

UI::UI()
  : db(std::make_shared<DB_SQLite>("timesheeting.db")),
    projects_screen(db),
    locations_screen(db),
    entries_screen(db),
    project_report_screen(db),
    weekly_report_screen(db) {
  logger().log("timesheeting UI initialized.");
}

UI::~UI() {}

char UI::input_loop() {
  UIComponent *cur_screen {&projects_screen};
  projects_screen.clear();
  locations_screen.clear();
  entries_screen.clear();
  project_report_screen.clear();
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
    case '4':
      cur_screen = &project_report_screen;
      break;
    case '5':
      cur_screen = &weekly_report_screen;
      break;
    default:
      return ch;
    }
  }
}
