/** @file
 * @brief Top-level UI of the application. */
#ifndef UI_H
#define UI_H

#include "ui_screens/entries_screen.h"
#include "ncurses/ncurses_handle.h"
#include "ui_screens/locations_screen.h"
#include "ui_screens/project_report_screen.h"
#include "ui_screens/project_task_screen.h"
#include "ui_screens/weekly_report_screen.h"

/** @brief Ncurses top-level UI for timesheeting. */
class UI {
public:
  /** @brief Construct the timesheeting top-level UI. */
  UI ();
  /** @brief Destructor. */
  ~UI ();
  /** @brief Top-level input loop. */
  char input_loop();

private:
  /** @brief Handle to the top ncurses management. */
  NcursesHandle ncurses_handle;
  /** @brief Screen for the project/task definition. */
  ProjectTaskScreen projects_screen;
  /** @brief Screen for the locations definition. */
  LocationsScreen locations_screen;
  /** @brief Screen for the table of entries. */
  EntriesScreen entries_screen;
  /** @brief Screen for the project totals report. */
  ProjectReportScreen project_report_screen;
  /** @brief Weekly report. */
  WeeklyReportScreen weekly_report_screen;
};

#endif // UI_H
