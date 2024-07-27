#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column_interface.h"
#include "db_interface.h"
#include <memory>
#include <algorithm>

/** @brief Table for defining projects and tasks. */
template <typename T_DB, typename T_PROJ, typename T_TASK,
          typename = std::enable_if_t<
            std::is_base_of<DB_Interface, T_DB>::value &&
            std::is_base_of<ColumnInterface<Project>, T_PROJ>::value &&
            std::is_base_of<ColumnInterface<Task>, T_TASK>::value>>
class ProjectTaskTable {
public:
  explicit ProjectTaskTable(std::shared_ptr<T_DB> _db)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      project_col (std::make_unique<T_PROJ>(db->query_projects(), ColPos::left)),
      task_col (std::make_unique<T_TASK>(
        db->query_tasks(project_col->get_current_id()), ColPos::middle)) {}

  /** Query an user input, treat it or return it. */
  char input_loop() {
    ColumnInterfaceBase *cur_col {project_col.get()};
    while (true) {
      cur_col->refresh();
      auto ch = cur_col->query_input();
      switch (ch) {
      case 'n':
        cur_col->select_next_item();
        update_task_col(cur_col);
        break;
      case 'e':
        cur_col->select_prev_item();
        update_task_col(cur_col);
        break;
      case 'h':
        cur_col = project_col.get();
        break;
      case 'i':
        cur_col = task_col.get();
        break;
      case 'a': // Add project.
      {
        auto project_name = project_col->query_new_item_name();
        auto sanitized_project_name = sanitize_input(project_name);
        if (!sanitized_project_name.empty()) {
          db->add_project(sanitized_project_name);
        }
        // Update the project column.
        auto project_items = db->query_projects();
        project_col->set_items(project_items);
    } break;
    case 'r': // Edit project name.
    {
      auto id = project_col->get_current_id();
      auto new_project_name = project_col->query_current_item_rename();
      auto sanitized_project_name =
        sanitize_input(new_project_name);
      if (!sanitized_project_name.empty()) {
        db->edit_project_name(id, sanitized_project_name);
      }
      // Update the project column.
      auto project_items = db->query_projects();
      project_col->set_items(project_items);
    } break;
    // TODO: * Remove project.
    default:
      return ch;
    }
  }
  }

private:
  std::shared_ptr<DB_Interface> db;
  std::unique_ptr<ColumnInterface<Project>> project_col;
  std::unique_ptr<ColumnInterface<Task>> task_col;

  // Update the task column if we are on the project column.
  void update_task_col(ColumnInterfaceBase *cur_col) {
    if (cur_col == project_col.get()) {
      Id cur_project = project_col->get_current_id();
      auto task_items = db->query_tasks(cur_project);
      task_col->set_items(task_items);
    }
  }

  /** Sanitize the queried user input strings. */
  std::string sanitize_input(std::string input) {
    auto s = input;
    // left trim
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         [](unsigned char ch) {
                           return !std::isspace(ch);}));
    // right trim
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) {
                           return !std::isspace(ch);}).base(), s.end());
    return s;
  }
};

#endif // PROJECT_TASK_TABLE_H
