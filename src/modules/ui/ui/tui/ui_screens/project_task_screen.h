/** @file
 * @brief Projects and tasks screen UI. */
#ifndef PROJECT_TASK_TABLE_H
#define PROJECT_TASK_TABLE_H

#include "column/column.h"
#include "column/column_base.h"
#include "ui_screen.h"
#include <memory>
#include "core/generic_item.h"

namespace tui {
  /** @brief Projects and tasks UI screen. */
  class ProjectTaskScreen final : public UIScreen {
  public:
    /** @brief Table constructor. */
    explicit ProjectTaskScreen();

    void refresh () final;
    void clear () final;
    void resize () final;
    config::NormalActions input_loop() final;
    void update() final;
    void update_status() final;

  private:
    /** @brief Column for projects. */
    std::unique_ptr<Column<core::Project>> project_col;
    /** @brief Column for tasks. */
    std::unique_ptr<Column<core::Task>> task_col;
    /** @brief Current column in focus. */
    ColumnBase *cur_col;
    /** @brief Whether to show only active items. */
    bool show_only_active;

    /** @brief Update the tasks column. */
    void update_task_col();
    /** @brief Update the project column. */
    void update_project_col();
    /** @brief Add an item (project or task). */
    bool add_item (const ColumnBase *cur_col);
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
} // namespace tui

#endif // PROJECT_TASK_TABLE_H
