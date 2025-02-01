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
- DB version updated to 2.
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
