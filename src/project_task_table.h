#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column.h"
#include "db_interface.h"
#include <iostream> // TODO temp

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

  // Update the task column if we are on the project column.
  void update_task_col(ColumnBase *cur_col);
};

#endif // PROJECT_TASK_TABLE_H
