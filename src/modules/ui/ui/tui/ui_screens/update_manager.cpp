#include "update_manager.h"
#include "ui/tui/ui_screens/status_bar/status_bar.h"
#include <stdexcept>

namespace tui {
  UpdateManager &UpdateManager::get(
      std::shared_ptr<ProjectTaskScreen> _projects_screen,
      std::shared_ptr<LocationsScreen> _locations_screen,
      std::shared_ptr<EntriesScreen> _entries_screen,
      std::shared_ptr<ProjectReportScreen> _project_report_screen,
      std::shared_ptr<WeeklyReportScreen> _weekly_report_screen) {
    static UpdateManager instance {_projects_screen,
                                   _locations_screen,
                                   _entries_screen,
                                   _project_report_screen,
                                   _weekly_report_screen};
    return instance;
  }

  UpdateManager::UpdateManager(
      std::shared_ptr<ProjectTaskScreen> _projects_screen,
      std::shared_ptr<LocationsScreen> _locations_screen,
      std::shared_ptr<EntriesScreen> _entries_screen,
      std::shared_ptr<ProjectReportScreen> _project_report_screen,
      std::shared_ptr<WeeklyReportScreen> _weekly_report_screen)
      : projects_screen(_projects_screen), locations_screen(_locations_screen),
        entries_screen(_entries_screen),
        project_report_screen(_project_report_screen),
        weekly_report_screen(_weekly_report_screen),
        current_focus(_entries_screen) {
    if (_projects_screen == nullptr or _locations_screen == nullptr or
        _entries_screen == nullptr or _project_report_screen == nullptr or
        _weekly_report_screen == nullptr) {
      throw std::runtime_error("All screens are not initialized.");
    }
  }

  void UpdateManager::entries_have_changed() {
    project_report_screen->signal_update_need();
    weekly_report_screen->signal_update_need();
  }

  void UpdateManager::projects_tasks_have_changed() {
    entries_screen->signal_update_need();
    project_report_screen->signal_update_need();
    weekly_report_screen->signal_update_need();
  }

  void UpdateManager::locations_have_changed() {
    entries_screen->signal_update_need();
  }

  void UpdateManager::duration_display_changed () {
    entries_screen->signal_update_need();
    project_report_screen->signal_update_need();
    weekly_report_screen->signal_update_need();
  }

  void UpdateManager::terminal_was_resized () {
    projects_screen->signal_resize_need();
    locations_screen->signal_resize_need();
    entries_screen->signal_resize_need();
    project_report_screen->signal_resize_need();
    weekly_report_screen->signal_resize_need();
    status().resize();
    current_focus->refresh();
    current_focus->update_status();
  }
} // namespace tui
