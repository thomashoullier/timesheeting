#include "project_task_screen.h"
#include "config/key.h"
#include "db/db_sqlite.h"
#include "status_bar/status_bar.h"
#include "log_lib/logger.h"
#include "update_manager.h"
#include <stdexcept>
#include "ui/keys/bound_keys.h"

namespace tui {
  ProjectTaskScreen::ProjectTaskScreen()
    : project_col(std::make_unique<Column<core::Project>>
                  (std::vector<core::Project>(),
                   ncurses_lib::WindowPosition::left)),
      task_col(std::make_unique<Column<core::Task>>
               (std::vector<core::Task>(),
                ncurses_lib::WindowPosition::middle)),
      show_only_active(true) {
    // TODO: initialize directly on actual db data.
    update_project_col();
    update_task_col();
  }

  void ProjectTaskScreen::refresh() {
    project_col->refresh();
    task_col->refresh();
  }

  void ProjectTaskScreen::clear() {
    project_col->clear();
    task_col->clear();
  }

  void ProjectTaskScreen::resize() {
    project_col->resize();
    task_col->resize();
  }

  config::NormalActions ProjectTaskScreen::input_loop() {
    ColumnBase *cur_col{project_col.get()};
    cur_col->set_border();
    while (true) {
      status().print(cur_col->get_current_item_string());
      auto ch = cur_col->get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = keys::BoundKeys::get().kb.normal_mode.action_requested(ch);
      switch(action) {
      case config::NormalActions::down:
        cur_col->select_down_item();
        if (cur_col == project_col.get()) {
          update_task_col();
        }
        break;
      case config::NormalActions::up:
        cur_col->select_up_item();
        if (cur_col == project_col.get()) {
          update_task_col();
        }
        break;
      case config::NormalActions::left:
        cur_col->unset_border();
        cur_col = project_col.get();
        cur_col->set_border();
        break;
      case config::NormalActions::right:
        cur_col->unset_border();
        cur_col = task_col.get();
        cur_col->set_border();
        break;
      case config::NormalActions::add:
        if (not(add_item(cur_col))) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        } else {
          UpdateManager::get().projects_tasks_have_changed();
        }
        break;
      case config::NormalActions::rename:
        if (not(rename_item(cur_col))) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        } else {
          UpdateManager::get().projects_tasks_have_changed();
        }
        break;
      case config::NormalActions::task_project_change:
        if (not(change_task_project(cur_col))) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        } else {
          UpdateManager::get().projects_tasks_have_changed();
        }
        break;
      case config::NormalActions::remove:
        remove_item(cur_col);
        break;
      case config::NormalActions::active_toggle:
        toggle_active_item(cur_col);
        break;
      case config::NormalActions::active_visibility:
        toggle_archive_visibility();
        break;
      default:
        cur_col->unset_border();
        return action;
      }
    }
  }

  void ProjectTaskScreen::update() {
    log_lib::logger().log("ProjectTaskScreen update.", log_lib::LogLevel::debug);
    update_project_col();
    update_task_col();
  }

  void ProjectTaskScreen::update_task_col() {
    try {
      core::Id cur_project = project_col->get_current_id();
      std::vector<core::Task> task_items;
      if (show_only_active)
        task_items = db::db().query_tasks_active(cur_project);
      else
        task_items = db::db().query_tasks(cur_project);
      task_col->set_items(task_items);
      task_col->refresh();
    } catch (ncurses_lib::MenuEmpty &e) {
      return;
    }
  }

  void ProjectTaskScreen::update_project_col() {
    std::vector<core::Project> project_items;
    if (show_only_active)
      project_items = db::db().query_projects_active();
    else
      project_items = db::db().query_projects();
    project_col->set_items(project_items);
    project_col->refresh();
  }

  bool ProjectTaskScreen::add_item(ColumnBase *cur_col) {
    // TODO: rewrite, this got ugly.
    auto new_item_name = status().get_user_string();
    if (new_item_name.empty())
      return true;
    if (cur_col == project_col.get()) {
      auto success = db::db().add_project(new_item_name);
      log_lib::logger().log("Added project: " + new_item_name,
                            log_lib::LogLevel::info);
      update_project_col();
      return success;
    } else if (cur_col == task_col.get()) {
      try {
        auto project_id = project_col->get_current_id();
        auto success = db::db().add_task(project_id, new_item_name);
        log_lib::logger().log("Added task: " + new_item_name,
                              log_lib::LogLevel::info);
        update_task_col();
        return success;
      } catch (ncurses_lib::MenuEmpty &e) {
        return true;
      }
    }
    return true;
  }

  bool ProjectTaskScreen::rename_item(ColumnBase *cur_col) {
    try {
      auto id = cur_col->get_current_id();
      auto new_item_name = status().get_user_string();
      if (!new_item_name.empty()) {
        if (cur_col == project_col.get()) {
          auto success = db::db().edit_project_name(id, new_item_name);
          update_project_col();
          return success;
        } else if (cur_col == task_col.get()) {
          auto success = db::db().edit_task_name(id, new_item_name);
          update_task_col();
          return success;
        }
      }
      return true;
    } catch (ncurses_lib::MenuEmpty &e) {
      return true;
    }
  }

  bool ProjectTaskScreen::change_task_project(ColumnBase *cur_col) {
    if (cur_col == project_col.get())
      return true; // Do nothing while in the project column.
    if (cur_col != task_col.get())
      throw std::runtime_error("change_task_project: Unknown column type.");
    try {
      auto id = cur_col->get_current_id();
      auto new_project_name = status().get_user_string();
      if (!new_project_name.empty()) {
        auto success = db::db().edit_task_project(id, new_project_name);
        update_task_col();
        return success;
      }
      return true;
    } catch (ncurses_lib::MenuEmpty &e) {
      return true;
    }
  }

  void ProjectTaskScreen::remove_item(ColumnBase *cur_col) {
    try {
      auto id = cur_col->get_current_id();
      bool user_conf = status().query_confirmation("Remove item? (Y/N)");
      if (!user_conf) {
        return;
      }
      if (cur_col == task_col.get()) {
        db::db().delete_task(id);
        update_task_col();
      } else if (cur_col == project_col.get()) {
        db::db().delete_project(id);
        update_project_col();
        update_task_col();
      }
    } catch (ncurses_lib::MenuEmpty &e) {
      return;
    }
  }

  void ProjectTaskScreen::toggle_active_item(ColumnBase *cur_col) {
    try {
      auto id = cur_col->get_current_id();
      if (cur_col == task_col.get()) {
        db::db().toggle_task_active(id);
        update_task_col();
      } else if (cur_col == project_col.get()) {
        db::db().toggle_project_active(id);
        update_project_col();
        update_task_col();
      }
    } catch (ncurses_lib::MenuEmpty &e) {
      return;
    }
  }

  void ProjectTaskScreen::toggle_archive_visibility() {
    show_only_active = !show_only_active;
    update_project_col();
    update_task_col();
  }
} // namespace tui
