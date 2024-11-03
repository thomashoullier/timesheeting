/** @file
 * @brief Projects and tasks screen UI. */
#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column/column.h"
#include "ui_screen.h"
#include <memory>

/** @brief Projects and tasks UI screen. */
class ProjectTaskScreen : public UIScreen {
public:
  /** @brief Table constructor. */
  explicit ProjectTaskScreen();

  void refresh () override;
  void clear () override;
  char input_loop() override;
  void update() override;

private:
  /** @brief Column for projects. */
  std::unique_ptr<Column<Project>> project_col;
  /** @brief Column for tasks. */
  std::unique_ptr<Column<Task>> task_col;
  /** @brief Whether to show only active items. */
  bool show_only_active;

  /** @brief Update the tasks column. */
  void update_task_col();
  /** @brief Update the project column. */
  void update_project_col();
  /** @brief Add an item (project or task). */
  bool add_item (ColumnBase *cur_col);
  /** @brief Rename an item. */
  bool rename_item(ColumnBase *cur_col);
  /** @brief Change a task's project. */
  bool change_task_project(ColumnBase *cur_col);
  /** @brief Remove the currently selected item. */
  void remove_item (ColumnBase *cur_col);
  /** @brief Toggle the active flag on the selected item. */
  void toggle_active_item (ColumnBase *cur_col);
  /** @brief Toggle archive items visibility. */
  void toggle_archive_visibility ();
};

#endif // PROJECT_TASK_TABLE_H
