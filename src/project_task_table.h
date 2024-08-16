/** @file
 * @brief Projects and tasks screen UI. */
#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column_interface.h"
#include "data_objects.h"
#include "db_interface.h"
#include "logger_interface.h"
#include "status_bar_interface.h"
#include "ui_screen.h"
#include <memory>
#include <algorithm>

/** @brief Table for defining projects and tasks using two columns. */
template <typename T_DB, typename T_ST,
          typename T_PROJ, typename T_TASK,
          typename T_LOG,
          typename = std::enable_if_t<
            std::is_base_of<DB_Interface, T_DB>::value &&
            std::is_base_of<StatusBarInterface, T_ST>::value &&
            std::is_base_of<ColumnInterface<Project>, T_PROJ>::value &&
            std::is_base_of<ColumnInterface<Task>, T_TASK>::value &&
            std::is_base_of<LoggerInterface, T_LOG>::value>>
class ProjectTaskTable : public UIScreen {
public:
  /** @brief Table constructor. */
  explicit ProjectTaskTable(std::shared_ptr<T_DB> _db,
                            std::shared_ptr<T_ST> _status)
      : db(std::static_pointer_cast<DB_Interface>(_db)),
        status(std::static_pointer_cast<StatusBarInterface>(_status)),
        project_col(
            std::make_unique<T_PROJ>(std::vector<Project>(), ColPos::left)),
        task_col(std::make_unique<T_TASK>(std::vector<Task>(),
                                          ColPos::middle)),
        logger(&T_LOG::get()) {
    // TODO: initialize directly on actual db data.
    update_project_col();
    update_task_col();
  }

  void refresh () override {
    project_col->refresh();
    task_col->refresh();
  };

  void clear () override {
    project_col->clear();
    task_col->clear();
  };

  /** @brief Query an user input, treat it or return it. */
  char input_loop() override {
    ColumnInterfaceBase *cur_col {project_col.get()};
    while (true) {
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
          status->print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      case 'r':
        try {
          rename_item(cur_col);
        }
        catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      case 'x':
        try {
          remove_item(cur_col);
        }
        catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      default:
        return ch;
      }
    }
  }

private:
  /** @brief Interface to the DB. */
  std::shared_ptr<DB_Interface> db;
  /** @brief Interface to the status bar. */
  std::shared_ptr<StatusBarInterface> status;
  /** @brief Column for projects. */
  std::unique_ptr<ColumnInterface<Project>> project_col;
  /** @brief Column for tasks. */
  std::unique_ptr<ColumnInterface<Task>> task_col;
  /** @brief Pointer to the logger singleton. */
  LoggerInterface *logger;

  /** @brief Update the tasks column. */
  void update_task_col() {
    try {
      Id cur_project = project_col->get_current_id();
      auto task_items = db->query_tasks(cur_project);
      task_col->set_items(task_items);
      task_col->refresh();
    } catch (ColumnEmpty &e) {
      return;
    }
  }

  /** @brief Update the project column. */
  void update_project_col() {
    auto project_items = db->query_projects();
    project_col->set_items(project_items);
    project_col->refresh();
  }

  /** @brief Sanitize the queried user input strings. */
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

  /** @brief Add an item (project or task). */
  void add_item (ColumnInterfaceBase *cur_col) {
    auto new_item_name = cur_col->query_new_item_name();
    auto sanitized_item_name = sanitize_input(new_item_name);
    if (!sanitized_item_name.empty()) {
      if (cur_col == project_col.get()) {
        db->add_project(sanitized_item_name);
        logger->log("Added project: " + sanitized_item_name);
        update_project_col();
      } else if (cur_col == task_col.get()) {
        try {
        auto project_id = project_col->get_current_id();
        db->add_task(project_id, sanitized_item_name);
        logger->log("Added task: " + sanitized_item_name);
        update_task_col();
        } catch (ColumnEmpty &e) {
          return;
        }
      }
    }
  }

  /** @brief Rename an item. */
  void rename_item (ColumnInterfaceBase *cur_col) {
    try {
      auto id = cur_col->get_current_id();
      auto new_item_name = cur_col->query_current_item_rename();
      auto sanitized_item_name = sanitize_input(new_item_name);
      if (!sanitized_item_name.empty()) {
        if (cur_col == project_col.get()) {
          db->edit_project_name(id, sanitized_item_name);
          update_project_col();
        } else if (cur_col == task_col.get()) {
          db->edit_task_name(id, sanitized_item_name);
          update_task_col();
        }
      }
    } catch (ColumnEmpty &e) {
      return;
    }
  }

  /** @brief Remove the currently selected item. */
  void remove_item (ColumnInterfaceBase *cur_col) {
    try {
      auto id = cur_col->get_current_id();
      bool user_conf = status->query_confirmation("Remove item? (Y/N)");
      if (!user_conf) {
        return;
      }
      if (cur_col == task_col.get()) {
        db->delete_task(id);
        update_task_col();
      } else if (cur_col == project_col.get()) {
        db->delete_project(id);
        update_project_col();
        update_task_col();
      }
    } catch (ColumnEmpty &e) {
      return;
    }
  }
};

#endif // PROJECT_TASK_TABLE_H
