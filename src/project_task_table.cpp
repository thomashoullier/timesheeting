#include "project_task_table.h"

void ProjectTaskTable::update_task_col(ColumnBase *cur_col) {
  if (cur_col == &project_col) {
    Id cur_project = cur_col->get_field_id();
    auto task_items = db->query_tasks(cur_project);
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
    case 'a': // Add project.
      {
        auto project_name = project_col.input_new_item();
        auto sanitized_project_name = project_col.sanitize_input(project_name);
        if (!sanitized_project_name.empty()) {
          db->add_project(sanitized_project_name);
        }
        // Update the project column.
        auto project_items = db->query_projects();
        project_col.recreate_form(project_items);
    } break;
    case 'r': // Edit project name.
    {
      auto id = project_col.get_field_id();
      auto new_project_name = project_col.edit_current_item();
      auto sanitized_project_name =
        project_col.sanitize_input(new_project_name);
      if (!sanitized_project_name.empty()) {
        db->edit_project_name(id, sanitized_project_name);
      }
      // Update the project column.
      auto project_items = db->query_projects();
      project_col.recreate_form(project_items);
    } break;
    // TODO: * Remove project.
    default:
      return ch;
    }
  }
}
