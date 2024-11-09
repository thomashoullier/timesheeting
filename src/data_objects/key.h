/** @file
   @brief Representations for key bindings. */

struct Key {
  /** @brief Primary key for binding. */
  char primary;
  /** @brief Alternative key for binding. */
  char alt;
};

/** @brief Represents the key bindings in the program. */
struct KeyBindings {
  /** @brief Navigate up. */
  Key up;
  /** @brief Navigate down. */
  Key down;
  /** @brief Navigate left. */
  Key left;
  /** @brief Navigate right. */
  Key right;
  /** @brief Switch between subtabs. */
  Key subtabs;
  /** @brief Navigate previous. */
  Key previous;
  /** @brief Navigate next. */
  Key next;
  /** @brief Validate. */
  Key validate;
  /** @brief Cancel. */
  Key cancel;
  /** @brief Cycle between duration display formats. */
  Key duration_display;
  /** @brief Switch to entries screen. */
  Key entries_screen;
  /** @brief Switch to projects screen. */
  Key projects_screen;
  /** @brief Switch to locations screen. */
  Key locations_screen;
  /** @brief Switch to project report screen. */
  Key project_report_screen;
  /** @brief Switch to weekly report screen. */
  Key weekly_report_screen;
  /** @brief Adding objects. */
  Key add;
  /** @brief Renaming objects. */
  Key rename;
  /** @brief Remove objects. */
  Key remove;
  /** @brief Toggle the active/inactive state of objects. */
  Key active_toggle;
  /** @brief Toggle active/inactive objects visibility. */
  Key active_visibility;
  /** @brief Change the project a task is assigned to. */
  Key task_project_change;
};
