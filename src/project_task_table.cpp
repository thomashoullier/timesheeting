#include "project_task_table.h"

ProjectTaskTable::ProjectTaskTable()
  : project_col({""}, Column::left),
    task_col({""}, Column::middle) {
  auto projects_str = db.query_projects();
  project_col.recreate_form(projects_str);
  // TODO: implement ids for projects.
  // We need to put ids into the fields buffers, and read
  // the one which is selected.
  auto tasks_str = db.query_tasks(0);
  task_col.recreate_form(tasks_str);
}

char ProjectTaskTable::input_loop () {
  return wgetch(project_col.win);
}
