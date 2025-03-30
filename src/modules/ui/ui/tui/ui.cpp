#include "ui.h"
#include <ncurses.h>
#include <signal.h>
#include "config/key.h"
#include "ncurses_lib/win_ncurses.h"
#include "time_lib/duration_displayer.h"
#include "log_lib/logger.h"
#include <memory>
#include <string>
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
    // Install the SIGWINCH handler, overriding the default ncurses one
    signal(SIGWINCH, resize_handler);
  }

  int UI::input_loop() {
    projects_screen->clear();
    locations_screen->clear();
    entries_screen->clear();
    project_report_screen->clear();
    weekly_report_screen->clear();
    while (true) {
      UpdateManager::get().current_focus->refresh();
      auto action = UpdateManager::get().current_focus->input_loop();
      UpdateManager::get().current_focus->clear();
      switch(action) {
      case config::NormalActions::entries_screen:
        UpdateManager::get().current_focus = entries_screen;
        break;
      case config::NormalActions::projects_screen:
        UpdateManager::get().current_focus = projects_screen;
        break;
      case config::NormalActions::locations_screen:
        UpdateManager::get().current_focus = locations_screen;
        break;
      case config::NormalActions::project_report_screen:
        UpdateManager::get().current_focus = project_report_screen;
        break;
      case config::NormalActions::weekly_report_screen:
        UpdateManager::get().current_focus = weekly_report_screen;
        break;
      case config::NormalActions::duration_display:
        time_lib::DurationDisplayer::get().cycle_format();
        UpdateManager::get().duration_display_changed();
        break;
      case config::NormalActions::quit:
        return 0; // Return to main
      default: // Do nothing
        break;
      }
    }
  }

  void resize_handler(int sig) {
    auto sig_str = std::to_string(sig);
    log_lib::logger().log("SIGWINCH caught: " + sig_str,
                          log_lib::LogLevel::debug);
    // TODO: create an abstraction for this. Maybe on the NcursesHandle.
    erase();
    endwin();
    refresh();
    UpdateManager::get().terminal_was_resized();
  }
} // namespace tui
