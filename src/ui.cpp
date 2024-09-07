#include "ui.h"
#include "data_objects/date.h"
#include "logger/logger.h"
#include "ui_screens/project_report_screen.h"

UI::UI()
  : db(std::make_shared<DB_SQLite>("timesheeting.db")),
    status_bar(std::make_shared<StatusBarNCurses>()),
    projects_screen(db, status_bar),
    locations_screen(db, status_bar),
    entries_screen(db, status_bar),
    project_report_screen(db, status_bar),
    weekly_report_screen(db, status_bar) {
  log("timesheeting UI initialized.");
  // TEMP testing the weekly totals report
  auto totals = db->report_weekly_totals(Date(DatePoint::WeekBegin));
  auto strings = totals.to_strings();
  log("Weekly totals: ");
  for (const auto &str : strings) {
    log(str);
  }
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
