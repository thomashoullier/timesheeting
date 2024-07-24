#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column.h"
#include "db_interface.h"
#include <memory>

/** @brief Table for defining projects and tasks. */
class ProjectTaskTable {
public:
  template <typename T,
            typename = std::enable_if_t<
              std::is_base_of<DB_Interface, T>::value>>
  explicit ProjectTaskTable(std::shared_ptr<T> _db)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      project_col({}, ColPos::left),
      task_col({}, ColPos::middle) {
    auto project_items = db->query_projects();
    project_col.recreate_form(project_items);
    auto task_items = db->query_tasks(0);
    task_col.recreate_form(task_items);
  }
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
