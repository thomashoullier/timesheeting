#ifndef UI_H
#define UI_H

#include "column/column_ncurses.h"
#include "entries_table.h"
#include "logger_interface.h"
#include "ncurses_handle.h"
#include "project_task_table.h"
#include "db/db_sqlite.h"
#include "status_bar/status_bar_ncurses.h"
#include "logger/logger_file.h"
#include <memory>

/** @brief Ncurses UI for timesheeting. */
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
  /** @brief SQLite DB interface. */
  std::shared_ptr<DB_SQLite> db;
  /** @brief Handle to a status bar for ncurses. */
  std::shared_ptr<StatusBarNCurses> status_bar;
  ProjectTaskTable<DB_SQLite, StatusBarNCurses, ColumnNcurses<Project>,
                   ColumnNcurses<Task>, LoggerFile> projects_table;
  /** @brief Logger interface. */
  LoggerInterface *logger;
  /** @brief Screen for the table of entries. */
  EntriesTable entries_table;
};

#endif // UI_H
