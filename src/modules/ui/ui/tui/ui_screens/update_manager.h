/** @file
 *  @brief UpdateManager definition. */
#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "entries_screen.h"
#include "locations_screen.h"
#include "project_report_screen.h"
#include "project_task_screen.h"
#include "ui/tui/ui_screens/ui_screen.h"
#include "weekly_report_screen.h"
#include <memory>

namespace tui {
  /** @brief Manages the update of UIScreens based on catching changes through a
      singleton interface and propagate the need for UIScreens to update based on
      these changes through handles to the UIScreens.

      Singleton pattern. */
  class UpdateManager {
  public:
    /** @brief Grab the update manager singleton interface.

        Initialize it on the first call. */
    static UpdateManager&
    get(std::shared_ptr<ProjectTaskScreen> _projects_screen = nullptr,
        std::shared_ptr<LocationsScreen> _locations_screen = nullptr,
        std::shared_ptr<EntriesScreen> _entries_screen = nullptr,
        std::shared_ptr<ProjectReportScreen> _project_report_screen = nullptr,
        std::shared_ptr<WeeklyReportScreen> _weekly_report_screen = nullptr);

    /** @brief Signal a change in the entries state. */
    void entries_have_changed ();
    /** @brief Signal a change in the state of projects or tasks. */
    void projects_tasks_have_changed ();
    /** @brief Signal a change in the state of locations. */
    void locations_have_changed ();
    /** @brief Signal a change in the DurationDisplayer format. */
    void duration_display_changed ();
    /** @brief Signal a terminal resize has happened. */
    void terminal_was_resized ();

    UpdateManager(UpdateManager const&) = delete;
    void operator=(UpdateManager const&) = delete;

  private:
    /** @brief Screen for the project/task definition. */
    std::shared_ptr<ProjectTaskScreen> projects_screen;
    /** @brief Screen for the locations definition. */
    std::shared_ptr<LocationsScreen> locations_screen;
    /** @brief Screen for the table of entries. */
    std::shared_ptr<EntriesScreen> entries_screen;
    /** @brief Screen for the project totals report. */
    std::shared_ptr<ProjectReportScreen> project_report_screen;
    /** @brief Weekly report. */
    std::shared_ptr<WeeklyReportScreen> weekly_report_screen;
  public:
    /** @brief Screen which is the current focus*/
    std::shared_ptr<UIScreen> current_focus;

  private:
    /** @brief Constructor with member initialization. */
    explicit UpdateManager
    (std::shared_ptr<ProjectTaskScreen> _projects_screen,
     std::shared_ptr<LocationsScreen> _locations_screen,
     std::shared_ptr<EntriesScreen> _entries_screen,
     std::shared_ptr<ProjectReportScreen> _project_report_screen,
     std::shared_ptr<WeeklyReportScreen> _weekly_report_screen);
  };
} // namespace tui


#endif // UPDATE_MANAGER_H
