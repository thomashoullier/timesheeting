#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column.h"
#include "db_interface.h"
#include <memory>
#include <iostream> // TODO temp

/** @brief Table for defining projects and tasks. */
class ProjectTaskTable {
public:
  ProjectTaskTable(std::shared_ptr<DB_Interface> _db);
  /** Query an user input, treat it or return it. */
  char input_loop ();

private:
  std::shared_ptr<DB_Interface> db;
  Column<Project> project_col;
  Column<Task> task_col;

  // Update the task column if we are on the project column.
  void update_task_col(ColumnBase *cur_col);
};

#endif // PROJECT_TASK_TABLE_H
