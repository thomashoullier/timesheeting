#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column.h"
#include "db_interface.h"

/** @brief Table for defining projects and tasks. */
class ProjectTaskTable {
public:
  ProjectTaskTable();
  /** Query an user input, treat it or return it. */
  char input_loop ();

private:
  DB_Interface db;
  Column<Project> project_col;
  Column<Task> task_col;
};

#endif // PROJECT_TASK_TABLE_H
