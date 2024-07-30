#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column_interface.h"
#include "db_interface.h"
#include "status_bar_interface.h"
#include <memory>
#include <algorithm>

/** @brief Table for defining projects and tasks. */
template <typename T_DB, typename T_ST,
          typename T_PROJ, typename T_TASK,
          typename = std::enable_if_t<
            std::is_base_of<DB_Interface, T_DB>::value &&
            std::is_base_of<StatusBarInterface, T_ST>::value &&
            std::is_base_of<ColumnInterface<Project>, T_PROJ>::value &&
            std::is_base_of<ColumnInterface<Task>, T_TASK>::value>>
class ProjectTaskTable {
public:
  explicit ProjectTaskTable(std::shared_ptr<T_DB> _db,
                            std::shared_ptr<T_ST> _status)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      status(std::static_pointer_cast<StatusBarInterface>(_status)),
      project_col (std::make_unique<T_PROJ>(db->query_projects(), ColPos::left)),
      task_col (std::make_unique<T_TASK>(
        db->query_tasks(project_col->get_current_id()), ColPos::middle)) {}

  /** Query an user input, treat it or return it. */
  char input_loop() {
    ColumnInterfaceBase *cur_col {project_col.get()};
    while (true) {
      cur_col->refresh();
      status->print(cur_col->get_current_name());
      auto ch = cur_col->query_input();
      switch (ch) {
      case 'n':
        cur_col->select_next_item();
        if (cur_col == project_col.get()) {
          update_task_col();
        }
        break;
      case 'e':
        cur_col->select_prev_item();
        if (cur_col == project_col.get()) {
          update_task_col();
        }
        break;
      case 'h':
        cur_col = project_col.get();
        break;
      case 'i':
        cur_col = task_col.get();
        break;
      case 'a':
        try {
          add_item(cur_col);
        }
        catch (DBLogicExcept &e) {
          // TODO: make this continue by pressing any key to actually
          //       display the message.
          status->print("DB logic error! Nothing was done to the DB.");
        }
        break;
      case 'r':
        try {
          rename_item(cur_col);
        }
        catch (DBLogicExcept &e) {
          status->print("DB logic error! Nothing was done to the DB.");
        }
        break;
        // TODO: * Remove project.
      default:
        return ch;
      }
    }
  }

private:
  std::shared_ptr<DB_Interface> db;
  std::shared_ptr<StatusBarInterface> status;
  std::unique_ptr<ColumnInterface<Project>> project_col;
  std::unique_ptr<ColumnInterface<Task>> task_col;

  // Update the task column
  void update_task_col() {
    Id cur_project = project_col->get_current_id();
    auto task_items = db->query_tasks(cur_project);
    task_col->set_items(task_items);
  }

  /** Update the project column with data from the DB. */
  void update_project_col() {
    auto project_items = db->query_projects();
    project_col->set_items(project_items);
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

  void add_item (ColumnInterfaceBase *cur_col) {
    auto new_item_name = cur_col->query_new_item_name();
    auto sanitized_item_name = sanitize_input(new_item_name);
    if (!sanitized_item_name.empty()) {
      if (cur_col == project_col.get()) {
        db->add_project(sanitized_item_name);
        update_project_col();
      } else if (cur_col == task_col.get()) {
        auto project_id = project_col->get_current_id();
        db->add_task(project_id, sanitized_item_name);
        update_task_col();
      }
    }
  }

  void rename_item (ColumnInterfaceBase *cur_col) {
    auto id = cur_col->get_current_id();
    auto new_item_name = cur_col->query_current_item_rename();
    auto sanitized_item_name = sanitize_input(new_item_name);
    if (!sanitized_item_name.empty()) {
      if (cur_col == project_col.get()) {
        db->edit_project_name(id, sanitized_item_name);
        update_project_col();
      } else if (cur_col == task_col.get()) {
        db->edit_task_name(id, sanitized_item_name);
        // Update task column
        update_task_col();
      }
    }
  }
};

#endif // PROJECT_TASK_TABLE_H
