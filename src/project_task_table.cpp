#include "project_task_table.h"

ProjectTaskTable::ProjectTaskTable()
  : project_col({}, ColPos::left),
    task_col({}, ColPos::middle) {
  auto project_items = db.query_projects();
  project_col.recreate_form(project_items);
  auto task_items = db.query_tasks(0);
  task_col.recreate_form(task_items);
}

void ProjectTaskTable::update_task_col(ColumnBase *cur_col) {
  if (cur_col == &project_col) {
    Id cur_project = cur_col->get_field_id();
    auto task_items = db.query_tasks(cur_project);
    task_col.recreate_form(task_items);
  }
}

char ProjectTaskTable::input_loop() {
  ColumnBase *cur_col{&project_col};
  while (true) {
    cur_col->refresh();
    auto ch = cur_col->get_input();
    switch (ch) {
    case 'n':
      cur_col->next_field();
      update_task_col(cur_col);
      break;
    case 'e':
      cur_col->prev_field();
      update_task_col(cur_col);
      break;
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
