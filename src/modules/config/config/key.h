/** @file
    @brief Representations for key bindings. */
#ifndef KEY_H
#define KEY_H

namespace config {
  /** @brief Enumeration of the normal-mode actions performed in the software. */
enum class NormalActions {
  // Navigation
  up,
  down,
  left,
  right,
  subtabs,
  previous,
  next,
  duration_display,
  entries_screen,
  projects_screen,
  locations_screen,
  project_report_screen,
  weekly_report_screen,
  active_visibility,
  quit,
  // Action
  commit_entry,
  set_now,
  add,
  rename,
  remove,
  active_toggle,
  task_project_change,
  // Default: unbound
  unbound
};

/** @brief Enumeration of the edit-mode actions performed in the software. */
enum class EditActions {
  backspace, // Special, this is not accessible to bindings.
  validate,
  cancel,
  select_suggestion,
  // Default: unbound
  unbound
};

} // namespace config

#endif // KEY_H
