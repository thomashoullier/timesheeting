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
  Column *cur_col {&project_col};
  while (true) {
    cur_col->refresh();
    auto ch = cur_col->input_loop();
    switch (ch) {
    case 'h':
      cur_col = &project_col;
      break;
    case 'i':
      cur_col = &task_col;
      break;
    default:
      return ch;
    }
  }
}
