#include "ui.h"
#include "ncurses_lib/win_ncurses.h"
#include "time_lib/duration_displayer.h"
#include "log_lib/logger.h"
#include <memory>
#include "ui_screens/update_manager.h"
#include "ui/keys/bound_keys.h"

namespace tui {
  UI::UI()
    : projects_screen(std::make_shared<ProjectTaskScreen>()),
      locations_screen(std::make_shared<LocationsScreen>()),
      entries_screen(std::make_shared<EntriesScreen>()),
      project_report_screen(std::make_shared<ProjectReportScreen>()),
      weekly_report_screen(std::make_shared<WeeklyReportScreen>()) {
    log_lib::logger().log("timesheeting UI initialized.",
                          log_lib::LogLevel::debug);
    // Initialize the update manager with handle to all the screens.
    UpdateManager::get(projects_screen,
                       locations_screen,
                       entries_screen,
                       project_report_screen,
                       weekly_report_screen);
  }

  int UI::input_loop() {
    std::shared_ptr<UIScreen> cur_screen {projects_screen};
    projects_screen->clear();
    locations_screen->clear();
    entries_screen->clear();
    project_report_screen->clear();
    while (true) {
      cur_screen->refresh();
      auto ch = cur_screen->input_loop();
      cur_screen->clear();
      auto kb = keys::BoundKeys::get().kb;
      if (kb.entries_screen.bound_to(ch)) {
        cur_screen = entries_screen;
      } else if (kb.projects_screen.bound_to(ch)) {
        cur_screen = projects_screen;
      } else if (kb.locations_screen.bound_to(ch)) {
        cur_screen = locations_screen;
      } else if (kb.project_report_screen.bound_to(ch)) {
        cur_screen = project_report_screen;
      } else if (kb.weekly_report_screen.bound_to(ch)) {
        cur_screen = weekly_report_screen;
      } else if (kb.duration_display.bound_to(ch)) {
        time_lib::DurationDisplayer::get().cycle_format();
        UpdateManager::get().duration_display_changed();
      } else if (ch == ncurses_lib::screen_resize_event) {
        log_lib::logger().log("Screen resize happened.",
                              log_lib::LogLevel::debug);
      } else if (kb.quit.bound_to(ch)) {
        return 0; // Return to main
      }
    }
  }
} // namespace tui
