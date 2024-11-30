/** @file
 * @brief Top-level UI of the application. */
#ifndef UI_H
#define UI_H

#include "ncurses_lib/ncurses_handle.h"
#include "ui_screens/entries_screen.h"
#include "ui_screens/locations_screen.h"
#include "ui_screens/project_report_screen.h"
#include "ui_screens/project_task_screen.h"
#include "ui_screens/weekly_report_screen.h"
#include <memory>

namespace tui {
  /** @brief Ncurses top-level UI for timesheeting. */
  class UI {
  public:
    /** @brief Construct the timesheeting top-level UI. */
    explicit UI ();
    /** @brief Top-level input loop. */
    int input_loop();

  private:
    /** @brief Handle to the top ncurses management. */
    ncurses_lib::NcursesHandle ncurses_handle;
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
  };

  /** @brief Signal handler for terminal resizing events. */
  void resize_handler(int num);
} // namespace tui

#endif // UI_H
