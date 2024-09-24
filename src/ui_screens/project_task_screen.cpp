#include "project_task_screen.h"
#include "../db/db_sqlite.h"
#include "status_bar/status_bar_ncurses.h"
#include "../logger/logger.h"

ProjectTaskScreen::ProjectTaskScreen()
    : project_col(std::make_unique<Column<Project>>(std::vector<Project>(),
                                                    WindowPosition::left)),
      task_col(std::make_unique<Column<Task>>(std::vector<Task>(),
                                              WindowPosition::middle)),
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

char ProjectTaskScreen::input_loop() {
  ColumnBase *cur_col{project_col.get()};
  cur_col->set_border();
  while (true) {
    status().print(cur_col->get_current_item_string());
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
      if (not(add_item(cur_col)))
        status().print_wait("DB logic error! Nothing was done to the DB.");
      break;
    case 'r':
      if (not(rename_item(cur_col)))
        status().print_wait("DB logic error! Nothing was done to the DB.");
      break;
    case 'x':
      remove_item(cur_col);
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

void ProjectTaskScreen::update() {
  update_project_col();
  update_task_col();
}

void ProjectTaskScreen::update_task_col() {
  try {
    Id cur_project = project_col->get_current_id();
    std::vector<Task> task_items;
    if (show_only_active)
      task_items = db().query_tasks_active(cur_project);
    else
      task_items = db().query_tasks(cur_project);
    task_col->set_items(task_items);
    task_col->refresh();
  } catch (MenuEmpty &e) {
    return;
  }
}

void ProjectTaskScreen::update_project_col() {
  std::vector<Project> project_items;
  if (show_only_active)
    project_items = db().query_projects_active();
  else
    project_items = db().query_projects();
  project_col->set_items(project_items);
  project_col->refresh();
}

bool ProjectTaskScreen::add_item(ColumnBase *cur_col) {
  // TODO: rewrite, this got ugly.
  auto new_item_name = status().get_user_string();
  if (new_item_name.empty())
    return true;
  if (cur_col == project_col.get()) {
    auto success = db().add_project(new_item_name);
    logger().log("Added project: " + new_item_name);
    update_project_col();
    return success;
  } else if (cur_col == task_col.get()) {
    try {
      auto project_id = project_col->get_current_id();
      auto success = db().add_task(project_id, new_item_name);
      logger().log("Added task: " + new_item_name);
      update_task_col();
      return success;
    } catch (MenuEmpty &e) {
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
        auto success = db().edit_project_name(id, new_item_name);
        update_project_col();
        return success;
      } else if (cur_col == task_col.get()) {
        auto success = db().edit_task_name(id, new_item_name);
        update_task_col();
        return success;
      }
    }
    return true;
  } catch (MenuEmpty &e) {
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
      db().delete_task(id);
      update_task_col();
    } else if (cur_col == project_col.get()) {
      db().delete_project(id);
      update_project_col();
      update_task_col();
    }
  } catch (MenuEmpty &e) {
    return;
  }
}

void ProjectTaskScreen::toggle_active_item(ColumnBase *cur_col) {
  try {
    auto id = cur_col->get_current_id();
    if (cur_col == task_col.get()) {
      db().toggle_task_active(id);
      update_task_col();
    } else if (cur_col == project_col.get()) {
      db().toggle_project_active(id);
      update_project_col();
      update_task_col();
    }
  } catch (MenuEmpty &e) {
    return;
  }
}

void ProjectTaskScreen::toggle_archive_visibility() {
  show_only_active = !show_only_active;
  update_project_col();
  update_task_col();
}
