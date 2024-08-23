/** @file
 * @brief Top-level UI of the application. */
#ifndef UI_H
#define UI_H

#include "ui_screens/entries_screen.h"
#include "logger_interface.h"
#include "ncurses/ncurses_handle.h"
#include "ui_screens/project_task_screen.h"
#include "db/db_sqlite.h"
#include "ui_screens/status_bar/status_bar_ncurses.h"
#include "logger/logger_file.h"
#include <memory>

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
  /** @brief Logger interface. */
  LoggerInterface *logger;
  /** @brief Handle to the top ncurses management. */
  NcursesHandle ncurses_handle;
  /** @brief SQLite DB interface. */
  std::shared_ptr<DB_SQLite> db;
  /** @brief Handle to a status bar for ncurses. */
  std::shared_ptr<StatusBarNCurses> status_bar;
  /** @brief Screen for the project/task definition. */
  ProjectTaskScreen<DB_SQLite, LoggerFile> projects_screen;
  /** @brief Screen for the table of entries. */
  EntriesScreen<DB_SQLite> entries_screen;
};

#endif // UI_H
