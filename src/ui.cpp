#include "ui.h"
#include "logger/logger.h"
#include <memory>
#include "ui_screens/update_manager.h"

UI::UI()
  : projects_screen(std::make_shared<ProjectTaskScreen>()),
    locations_screen(std::make_shared<LocationsScreen>()),
    entries_screen(std::make_shared<EntriesScreen>()),
    project_report_screen(std::make_shared<ProjectReportScreen>()),
    weekly_report_screen(std::make_shared<WeeklyReportScreen>()) {
  logger().log("timesheeting UI initialized.", LogLevel::debug);
  // Initialize the update manager with handle to all the screens.
  UpdateManager::get(projects_screen,
                     locations_screen,
                     entries_screen,
                     project_report_screen,
                     weekly_report_screen);
}

char UI::input_loop() {
  std::shared_ptr<UIScreen> cur_screen {projects_screen};
  projects_screen->clear();
  locations_screen->clear();
  entries_screen->clear();
  project_report_screen->clear();
  while (true) {
    cur_screen->refresh();
    auto ch = cur_screen->input_loop();
    cur_screen->clear();
    switch (ch) {
    case '1':
      cur_screen = entries_screen;
      break;
    case '2':
      cur_screen = projects_screen;
      break;
    case '3':
      cur_screen = locations_screen;
      break;
    case '4':
      cur_screen = project_report_screen;
      break;
    case '5':
      cur_screen = weekly_report_screen;
      break;
    default:
      return ch;
    }
  }
}
