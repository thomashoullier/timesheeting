/** @file
    @brief Representations for key bindings. */
#ifndef KEY_H
#define KEY_H

namespace config {
  /** @brief A single key binding. */
  struct Key {
    /** @brief Primary key for binding. */
    char primary = 0;
    /** @brief Alternative key for binding. */
    char alt = 0;

    /** @brief Is the provided character bound to this key? */
    bool bound_to (char c) {return (primary == c) or (alt == c);};
  };

  /** @brief Key bindings in the "navigation" section. */
  struct NavigationKeys {
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
    /** @brief Toggle active/inactive objects visibility. */
    Key active_visibility;
    /** @brief Quit the program. */
    Key quit;
  };

  /** @brief Key bindings in the "actions" section. */
  struct ActionsKeys {
    /** @brief Commit an entry from entrystaging to the entries in DB. */
    Key commit_entry;
    /** @brief Set date to now. */
    Key set_now;
    /** @brief Adding objects. */
    Key add;
    /** @brief Renaming objects. */
    Key rename;
    /** @brief Remove objects. */
    Key remove;
    /** @brief Toggle the active/inactive state of objects. */
    Key active_toggle;
    /** @brief Change the project a task is assigned to. */
    Key task_project_change;
  };

  /** @brief Key bindings in the "edit_mode" section. */
  struct EditModeKeys {
    /** @brief Validate. */
    Key validate;
    /** @brief Cancel. */
    Key cancel;
    /** @brief Select suggestion. */
    Key select_suggestion;
  };

  /** @brief Represents the key bindings in the program. */
  struct KeyBindings {
    NavigationKeys navigation;
    ActionsKeys actions;
    EditModeKeys edit_mode;
  };

} // namespace config

#endif // KEY_H
