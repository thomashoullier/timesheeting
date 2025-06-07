# Release notes

## **v0.1** (18FEB2024)
The top-level GUI has been drafted.

## **v0.3** (24NOV2024)
Critical features are all implemented. Usage starts.

## **v0.4** (01FEB2025)
The detailed subsystem specification and unit testing was initiated. Some
ergonomy fixes and features were added. The list is detailed below.

### Software
- Added the `config_lib` library.
- Distinguished between `validate` key in edit mode and `commit_entry` key on
  the stopwatch.
- Key bindings organization in the configuration file modified.
- Upon edit mode cancellation, the stopwatch now reverts to its precedent state
  instead of going blank.
- Fixed the DB condition for start < stop in entries. Every entry must now be
  at least 1 second long.
- **DB version updated to 2.**
- Fixed a bug in the default location of configuration file.
- Added possibility of binding multiple keys to one action.
- Added detection of duplicate key bindings.
- Added special keys for key bindings, including arrow keys.
- Changed the key parsing mechanism: hashmap instead of if/else ladder.
- Added the log rotation feature.

### Testing
- Unit tests suite initiated, the following are fully tested: `config_lib`,
  `time_lib`, `version`, `config`.
- Added coverage report of unit tests.
- Added a CI chain.
- The CI tests the latex compilation of the documentation.
- The CI tests the compilation of the program.
- The CI runs the unit tests suite.

### Documentation
- Initiated SRD (system requirements document),
  the following are fully specified: `config_lib`, `time_lib`, `version`, `config`.
- Initiated TST (test plan), the unit tests for the following are fully specified:
  `config_lib`, `time_lib`, `version`, `config`.

## **v0.5**  (22FEB2025)
This release is solely dedicated to the custom ncurses menu implementation.
Terminal resize is now handled by the program.

### Software
- Implemented a custom ncurses menu, replacing the native ncurses menu.h
  library.

## **v0.6** (23FEB2025)
Mainly ergonomy improvements.

### Software
- **DB version incremented from 2 to 3.**
- Removed a menu.h include leftover.
- Archived items are now displayed in italic.
- The DB now forbids the addition of entries with time periods overlapping that
  of other entries.
- The project totals report screen now lists all individual tasks instead of
  just projects.

## **v0.7** (06APR2025)

### Software
- Fixed display bug for menus with cell item strings bigger than the display
  size.
- Fixed total bar update bug in project totals report.
- Made every UI screen remember their subtab focus between input loops. This
  fixes the change of focus when hitting unbound keys, and when resizing.
- Update and refresh loops in the UI slightly improved.
- The stopwatch entry start is now set to the last stop date upon successful
  commit.
- ergonomy: we can now change the selected day in the entries screen from the
  stopwatch also.
- ergonomy: stopwatch dates may now be parsed using hh, hh:mm, hh:mm:ss using
  the currently selected day.
- Edit mode is now exited upon terminal resizing.
- Fixed a scrolling bug in ncurses menu when changing items.
- Fixed suggestion display going beyond the window width.
- Fixed display glitch: status bar emptying on resize.
- Various style and maintainability fixes.
- Updated the required cmake version.

### Testing
- Updated codecoverage cmake plugin.

### CI
- Configured dockerhub tokens to avoid rate limits.
- Added cppcheck to the CI.

### Documentation
- Small doxygen addition of missing docstrings.

## **v0.8** (07JUN2025)

### Software features and bugs
- db_lib: The SQLite error codes checking is now more systematic.
- The external timesheet data format dates now have an UTC offset indicated.
- Exotic dates (before 1970 and far off into the future) are now better managed
throughout the program. All years properly parsed by the standard library are
now correctly taken into account.
- Fixed entries table and project/task columns behavior when interacting with
empty menus.
- Forbade the adding of tasks when the projects column is empty.
- The start and stop dates of existing entries may now be edited with
shortcuts based on the currently selected day.
- Forbade the committing of entrystaging on a day which is not selected
in the entries screen.
- Forbade the modification of an existing entry start date to a date
belonging to a non-selected day in the entries screen.
- Fixed UI refresh problem in project task screen: When renaming a project and
  updating the project column, the task column was not updated.

### Software refactoring
- db_lib: Moved the DB optimization from handle destructor to constructor to
  avoid possible exceptions in the destructor.
- exporter module: now decoupled from the DB module.
- Removed copy and assignment functions for singletons, eliminating the risk
of misuse bugs.
- Removed the reliance on MenuEmpty exception in the TUI.
- Added Date comparison operators and utilities.
- Aligned UI types with ncurses types for characters.

### Testing
- All non-UI libraries and modules are tested in the unit tests suite.

### CI
- Use the binary gentoo package to speed up the environment build.
- Added doxygen and code coverage pages deployment directly in the
CI. No longer rely on github actions.
- Added a memory leak (valgrind) test over the unit tests execution.

### Documentation
- The non-UI specification and test documentation for libraries and modules
  is now complete.
