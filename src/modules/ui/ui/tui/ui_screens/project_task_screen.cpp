#include "project_task_screen.h"
#include "config/key.h"
#include "db/db_sqlite.h"
#include "ncurses_lib/menu_ncurses.h"
#include "status_bar/status_bar.h"
#include "log_lib/logger.h"
#include "update_manager.h"
#include <stdexcept>
#include "ui/keys/bound_keys.h"
#include <vector>
#include <string>
#include "config/binding_maps.h"
#include "core/id.h"
#include "ncurses_lib/win_ncurses.h"
#include "ui/tui/ui_screens/column/column.h"
#include "ui/tui/ui_screens/column/column_base.h"

namespace tui {
  ProjectTaskScreen::ProjectTaskScreen()
    : project_col(std::make_unique<Column<core::Project>>
                  (std::vector<core::Project>(),
                   ncurses_lib::WindowPosition::left)),
      task_col(std::make_unique<Column<core::Task>>
               (std::vector<core::Task>(),
                ncurses_lib::WindowPosition::middle)),
      cur_col{project_col.get()},
      show_only_active(true) {
    // TODO: initialize directly on actual db data.
    update_project_col();
    update_task_col();
  }

  void ProjectTaskScreen::refresh() {
    if (needs_resize)
      resize();
    project_col->refresh();
    task_col->refresh();
  }

  void ProjectTaskScreen::clear() {
    project_col->clear();
    task_col->clear();
  }

  void ProjectTaskScreen::resize() {
    log_lib::Logger::get().log("ProjectTaskScreen::resize() called.",
                               log_lib::LogLevel::debug);
    project_col->resize();
    task_col->resize();
    needs_resize = false;
  }

  config::NormalActions ProjectTaskScreen::input_loop() {
    cur_col->set_border();
    update_status();
    while (true) {
      auto ch = cur_col->get_input();
      auto action = keys::BoundKeys::get().kb.normal_mode.action_requested(ch);
      switch(action) {
      case config::NormalActions::down:
        if (cur_col->select_down_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          if (cur_col == project_col.get()) {
            update_task_col();
          }
          update_status();
        }
        break;
      case config::NormalActions::up:
        if (cur_col->select_up_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          if (cur_col == project_col.get()) {
            update_task_col();
          }
          update_status();
        }
        break;
      case config::NormalActions::left:
        if (cur_col == task_col.get()) {
          cur_col->unset_border();
          cur_col = project_col.get();
          cur_col->set_border();
          update_status();
        }
        break;
      case config::NormalActions::right:
        if (cur_col == project_col.get()) {
          cur_col->unset_border();
          cur_col = task_col.get();
          cur_col->set_border();
          update_status();
        }
        break;
      case config::NormalActions::add:
        if (add_item(cur_col))
          UpdateManager::get().projects_tasks_have_changed();
        update_status();
        break;
      case config::NormalActions::rename:
        if (rename_item(cur_col))
          UpdateManager::get().projects_tasks_have_changed();
        update_status();
        break;
      case config::NormalActions::task_project_change:
        if (change_task_project(cur_col))
          UpdateManager::get().projects_tasks_have_changed();
        update_status();
        break;
      case config::NormalActions::remove:
        remove_item(cur_col);
        update_status();
        break;
      case config::NormalActions::active_toggle:
        toggle_active_item(cur_col);
        update_status();
        break;
      case config::NormalActions::active_visibility:
        toggle_archive_visibility();
        update_status();
        break;
      case config::NormalActions::entries_screen:
      case config::NormalActions::locations_screen:
      case config::NormalActions::project_report_screen:
      case config::NormalActions::weekly_report_screen:
      case config::NormalActions::duration_display:
      case config::NormalActions::quit:
        cur_col->unset_border();
        return action;
        break;
      default: // Do nothing
        break;
      }
    }
  }

  void ProjectTaskScreen::update() {
    log_lib::logger().log("ProjectTaskScreen update.", log_lib::LogLevel::debug);
    update_project_col();
    update_task_col();
  }

  void ProjectTaskScreen::update_status() {
    status().print(cur_col->get_current_item_string());
  }

  void ProjectTaskScreen::update_task_col() {
    if (project_col->is_empty())
      return;
    core::Id cur_project = project_col->get_current_id();
    std::vector<core::Task> task_items;
    if (show_only_active)
      task_items = db::db().query_tasks_active(cur_project);
    else
      task_items = db::db().query_tasks(cur_project);
    task_col->set_items(task_items);
    task_col->refresh();
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

  bool ProjectTaskScreen::add_item(const ColumnBase *cur_col) {
    if (cur_col == project_col.get()) {
      auto new_item_name = status().get_user_string();
      if (new_item_name.empty())
        return false;
      if(db::db().add_project(new_item_name)) {
        log_lib::logger().log("Added project: " + new_item_name,
                              log_lib::LogLevel::info);
        update_project_col();
      } else {
        status().print_wait("DB logic error! Nothing was done to the DB.");
        return false;
      }
    } else if (cur_col == task_col.get()) {
      if (project_col->is_empty())
        return false;
      auto new_item_name = status().get_user_string();
      if (new_item_name.empty())
        return false;
      auto project_id = project_col->get_current_id();
      if(db::db().add_task(project_id, new_item_name)) {
        log_lib::logger().log("Added task: " + new_item_name,
                              log_lib::LogLevel::info);
        update_task_col();
      } else {
        status().print_wait("DB logic error! Nothing was done to the DB.");
        return false;
      }
    }
    return true;
  }

  bool ProjectTaskScreen::rename_item(ColumnBase *cur_col) {
    if (cur_col->is_empty())
      return false;
    auto id = cur_col->get_current_id();
    auto new_item_name = status().get_user_string();
    if (new_item_name.empty())
      return false;
    if (cur_col == project_col.get()) {
      if (db::db().edit_project_name(id, new_item_name)) {
        update_project_col();
        update_task_col();
      } else {
        status().print_wait("DB logic error! Nothing was done to the DB.");
        return false;
      }
    } else if (cur_col == task_col.get()) {
      if (db::db().edit_task_name(id, new_item_name)) {
        update_task_col();
      } else {
        status().print_wait("DB logic error! Nothing was done to the DB.");
        return false;
      }
    }
    return true;
  }

  bool ProjectTaskScreen::change_task_project(ColumnBase *cur_col) {
    if (cur_col == project_col.get())
      return false; // Do nothing while in the project column.
    if (cur_col != task_col.get())
      throw std::runtime_error("change_task_project: Unknown column type.");
    if (cur_col->is_empty())
      return false;
    auto id = cur_col->get_current_id();
    auto new_project_name = status().get_user_string();
    if (new_project_name.empty())
      return false;
    if (db::db().edit_task_project(id, new_project_name)) {
      update_task_col();
    } else {
      status().print_wait("DB logic error! Nothing was done to the DB.");
      return false;
    }
    return true;
  }

  void ProjectTaskScreen::remove_item(ColumnBase *cur_col) {
    if (cur_col->is_empty())
      return;
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
  }

  void ProjectTaskScreen::toggle_active_item(ColumnBase *cur_col) {
    if (cur_col->is_empty())
      return;
    auto id = cur_col->get_current_id();
    if (cur_col == task_col.get()) {
      db::db().toggle_task_active(id);
      update_task_col();
    } else if (cur_col == project_col.get()) {
      db::db().toggle_project_active(id);
      update_project_col();
      update_task_col();
    }
  }

  void ProjectTaskScreen::toggle_archive_visibility() {
    show_only_active = !show_only_active;
    update_project_col();
    update_task_col();
  }
} // namespace tui
