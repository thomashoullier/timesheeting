#include "project_task_table.h"

ProjectTaskTable::ProjectTaskTable()
  : project_col({}, ColPos::left),
    task_col({}, ColPos::middle) {
  auto project_items = db.query_projects();
  project_col.recreate_form(project_items);
  // TODO: implement ids for projects.
  // We need to put ids into the fields buffers, and read
  // the one which is selected.
  auto task_items = db.query_tasks(0);
  task_col.recreate_form(task_items);
}

char ProjectTaskTable::input_loop () {
  ColumnBase *cur_col {&project_col};
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
