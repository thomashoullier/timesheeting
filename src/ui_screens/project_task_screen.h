/** @file
 * @brief Projects and tasks screen UI. */
#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column/column.h"
#include "../data_objects/data_objects.h"
#include "../db_interface.h"
#include "../logger/logger.h"
#include "status_bar/status_bar_ncurses.h"
#include "ui_component.h"
#include "../ncurses/win_ncurses.h"
#include <memory>

/** @brief Projects and tasks UI screen. */
template <typename T_DB,
          typename = std::enable_if_t<
            std::is_base_of<DB_Interface, T_DB>::value>>
class ProjectTaskScreen : public UIComponent {
public:
  /** @brief Table constructor. */
  explicit ProjectTaskScreen(std::shared_ptr<T_DB> _db,
                             std::shared_ptr<StatusBarNCurses> _status)
      : db(std::static_pointer_cast<DB_Interface>(_db)),
        status(_status),
        project_col(
                    std::make_unique<Column<Project>>(std::vector<Project>(),
                                                      WindowPosition::left)),
        task_col(std::make_unique<Column<Task>>(std::vector<Task>(),
                                                WindowPosition::middle)),
        show_only_active(true) {
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

  char input_loop() override {
    ColumnBase *cur_col {project_col.get()};
    cur_col->set_border();
    while (true) {
      status->print(cur_col->get_current_item_string());
      auto ch = cur_col->get_input();
      switch (ch) {
      case 'n':
        cur_col->select_down_item();
        if (cur_col == project_col.get()) {
          update_task_col();
        }
        break;
      case 'e':
        cur_col->select_up_item();
        if (cur_col == project_col.get()) {
          update_task_col();
        }
        break;
      case 'h':
        cur_col->unset_border();
        cur_col = project_col.get();
        cur_col->set_border();
        break;
      case 'i':
        cur_col->unset_border();
        cur_col = task_col.get();
        cur_col->set_border();
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
      case 'b':
        toggle_active_item(cur_col);
        break;
      case '.':
        toggle_archive_visibility();
        break;
      default:
        cur_col->unset_border();
        return ch;
      }
    }
  }

  void update() override {
    update_project_col();
    update_task_col();
  };

private:
  /** @brief Interface to the DB. */
  std::shared_ptr<DB_Interface> db;
  /** @brief Interface to the status bar. */
  std::shared_ptr<StatusBarNCurses> status;
  /** @brief Column for projects. */
  std::unique_ptr<Column<Project>> project_col;
  /** @brief Column for tasks. */
  std::unique_ptr<Column<Task>> task_col;
  /** @brief Whether to show only active items. */
  bool show_only_active;

  /** @brief Update the tasks column. */
  void update_task_col() {
    try {
      Id cur_project = project_col->get_current_id();
      std::vector<Task> task_items;
      if (show_only_active)
        task_items = db->query_tasks_active(cur_project);
      else
        task_items = db->query_tasks(cur_project);
      task_col->set_items(task_items);
      task_col->refresh();
    } catch (MenuEmpty &e) {
      return;
    }
  }

  /** @brief Update the project column. */
  void update_project_col() {
    auto project_items = db->query_projects();
    project_col->set_items(project_items);
    project_col->refresh();
  }

  /** @brief Add an item (project or task). */
  void add_item (ColumnBase *cur_col) {
    auto new_item_name = status->get_user_string();
    if (!new_item_name.empty()) {
      if (cur_col == project_col.get()) {
        db->add_project(new_item_name);
        log("Added project: " + new_item_name);
        update_project_col();
      } else if (cur_col == task_col.get()) {
        try {
          auto project_id = project_col->get_current_id();
          db->add_task(project_id, new_item_name);
          log("Added task: " + new_item_name);
          update_task_col();
        } catch (MenuEmpty &e) {
          return;
        }
      }
    }
  }

  /** @brief Rename an item. */
  void rename_item(ColumnBase *cur_col) {
    try {
      auto id = cur_col->get_current_id();
      auto new_item_name = status->get_user_string();
      if (!new_item_name.empty()) {
        if (cur_col == project_col.get()) {
          db->edit_project_name(id, new_item_name);
          update_project_col();
        } else if (cur_col == task_col.get()) {
          db->edit_task_name(id, new_item_name);
          update_task_col();
        }
      }
    } catch (MenuEmpty &e) {
      return;
    }
  }

  /** @brief Remove the currently selected item. */
  void remove_item (ColumnBase *cur_col) {
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
    } catch (MenuEmpty &e) {
      return;
    }
  }

  /** @brief Toggle the active flag on the selected item. */
  void toggle_active_item (ColumnBase *cur_col) {
    try {
      auto id = cur_col ->get_current_id();
      if (cur_col == task_col.get()) {
        db->toggle_task_active(id);
        update_task_col();
      } else if (cur_col == project_col.get()) {
        // TODO
      }
    } catch (MenuEmpty &e) {
      return;
    }
  };

  /** @brief Toggle archive items visibility. */
  void toggle_archive_visibility () {
    show_only_active = !show_only_active;
    update_project_col();
    update_task_col();
  };
};

#endif // PROJECT_TASK_TABLE_H
