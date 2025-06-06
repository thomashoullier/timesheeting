cmake_minimum_required(VERSION 3.31)

project(timesheeting)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_FLAGS
  "-Wall -O2 -Wextra -Wformat=2 -Werror -Wpedantic -fsanitize=signed-integer-overflow -march=native")

option(build_tests "Build the test suite" OFF)
option(build_coverage "Build the test coverage report" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For development with CLANGD

set(TIMESHEETING_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Dependencies

## Manually set the ncurses libraries
if (EXISTS "/usr/lib/libncurses.so")
  set(CURSES_LIBRARIES "/usr/lib/libncurses.so;/usr/lib/libtinfo.so")
else ()
  set(CURSES_LIBRARIES "/usr/lib64/libncurses.so;/usr/lib64/libtinfo.so")
endif()
find_package (SQLite3 REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(rapidfuzz REQUIRED)

# Internal libraries
file(GLOB_RECURSE TIME_LIB_SRC "${TIMESHEETING_DIR}/src/libs/time_lib/*.cpp")
add_library(time_lib STATIC "${TIME_LIB_SRC}")

file(GLOB_RECURSE LOG_LIB_SRC "${TIMESHEETING_DIR}/src/libs/log_lib/*.cpp")
add_library(log_lib STATIC "${LOG_LIB_SRC}")
target_link_libraries(log_lib PUBLIC time_lib)
target_include_directories(log_lib PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>")

file(GLOB_RECURSE DB_LIB_SRC "${TIMESHEETING_DIR}/src/libs/db_lib/*.cpp")
add_library(db_lib STATIC "${DB_LIB_SRC}")
target_link_libraries(db_lib PRIVATE ${SQLite3_LIBRARIES})

file(GLOB_RECURSE NCURSES_LIB_SRC
     "${TIMESHEETING_DIR}/src/libs/ncurses_lib/*.cpp")
add_library(ncurses_lib STATIC "${NCURSES_LIB_SRC}")
target_link_libraries(ncurses_lib PRIVATE ${CURSES_LIBRARIES})

file(GLOB_RECURSE SUGGESTION_SRC "${TIMESHEETING_DIR}/src/libs/suggestion/*.cpp")
add_library(suggestion STATIC "${SUGGESTION_SRC}")
target_link_libraries(suggestion PRIVATE rapidfuzz::rapidfuzz)

file(GLOB_RECURSE CONFIG_LIB_SRC "${TIMESHEETING_DIR}/src/libs/config_lib/*.cpp")
add_library(config_lib STATIC "${CONFIG_LIB_SRC}")
target_link_libraries(config_lib PRIVATE tomlplusplus::tomlplusplus)

# Internal modules
file(GLOB_RECURSE CONFIG_SRC "${TIMESHEETING_DIR}/src/modules/config/*.cpp")
add_library(config STATIC "${CONFIG_SRC}")
target_link_libraries(config PUBLIC config_lib
                             PRIVATE ${CURSES_LIBRARIES})
target_include_directories(config PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/config_lib/>")

file(GLOB_RECURSE KEYS_SRC "${TIMESHEETING_DIR}/src/modules/ui/ui/keys/*.cpp")
add_library(keys STATIC "${KEYS_SRC}")
target_link_libraries(keys PUBLIC config)
target_include_directories(keys PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/config/>")

file(GLOB_RECURSE CORE_SRC "${TIMESHEETING_DIR}/src/modules/core/*.cpp")
add_library(core STATIC "${CORE_SRC}")
target_link_libraries(core PUBLIC time_lib ncurses_lib db_lib)
target_include_directories(core PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/ncurses_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/db_lib/>")

file(GLOB_RECURSE DB_SRC "${TIMESHEETING_DIR}/src/modules/db/*.cpp")
add_library(db STATIC "${DB_SRC}")
target_link_libraries(db PUBLIC db_lib time_lib core)
target_include_directories(db PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/version/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/db_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/core/>")

file(GLOB_RECURSE EXPORTER_SRC "${TIMESHEETING_DIR}/src/modules/exporter/*.cpp")
add_library(exporter STATIC "${EXPORTER_SRC}")
target_link_libraries(exporter PUBLIC time_lib core)
target_include_directories(exporter PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/core/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/version/>")

file(GLOB_RECURSE CLI_SRC "${TIMESHEETING_DIR}/src/modules/ui/ui/cli/*.cpp")
add_library(cli STATIC "${CLI_SRC}")
target_include_directories(cli PRIVATE /usr/include/tclap)
target_include_directories(cli PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/version/>")

file(GLOB_RECURSE TUI_SRC "${TIMESHEETING_DIR}/src/modules/ui/ui/tui/*.cpp")
add_library(tui STATIC "${TUI_SRC}")
target_link_libraries(tui PUBLIC ncurses_lib core
                      suggestion time_lib log_lib
                      keys db)
target_include_directories(tui PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/ncurses_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/core/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/suggestion/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/log_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/ui/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/db/>")

# Main
add_executable(timesheeting "${TIMESHEETING_DIR}/src/main.cpp")
target_link_libraries(timesheeting PUBLIC
                      keys cli tui exporter db time_lib log_lib
                      config)
target_include_directories(timesheeting PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/version/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/ui/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/exporter/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/db/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/log_lib/>"
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/config/>")

# Installation
set(CMAKE_INSTALL_PREFIX /)
install(TARGETS timesheeting DESTINATION bin)
install(FILES "${TIMESHEETING_DIR}/timesheeting.toml" DESTINATION etc)

if(build_tests OR build_coverage)
  # External dependencies
  find_package(Catch2 3 REQUIRED)
  # Test libraries
  file(GLOB_RECURSE TEST_UTILS_SRC
    "${TIMESHEETING_DIR}/test/test_utils/test_utils/*.cpp")
  add_library(test_utils STATIC "${TEST_UTILS_SRC}")
  # Test source
  # Order matters here.
  set(TEST_SRC "${TIMESHEETING_DIR}/test/libs/time_lib/time_lib_test.cpp"
    "${TIMESHEETING_DIR}/test/libs/config_lib/config_lib_test.cpp"
    "${TIMESHEETING_DIR}/test/libs/db_lib/db_lib_test.cpp"
    "${TIMESHEETING_DIR}/test/libs/suggestion_lib/suggestion_lib_test.cpp"
    "${TIMESHEETING_DIR}/test/libs/log_lib/log_lib_test.cpp"
    "${TIMESHEETING_DIR}/test/modules/version/version_test.cpp"
    "${TIMESHEETING_DIR}/test/modules/core/core_test.cpp"
    "${TIMESHEETING_DIR}/test/modules/config/config_test.cpp"
    "${TIMESHEETING_DIR}/test/modules/db/db_test.cpp"
    "${TIMESHEETING_DIR}/test/modules/exporter/exporter_test.cpp"
  )
endif()

# Unit tests
if(build_tests)
  add_executable(timesheeting_tests "${TEST_SRC}")
  target_link_libraries(timesheeting_tests
    PRIVATE Catch2::Catch2WithMain
    PUBLIC config_lib test_utils config time_lib suggestion db_lib core
    exporter db log_lib)
  target_include_directories(timesheeting_tests PUBLIC
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/test/test_utils/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/version/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/core/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/ui/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/exporter/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/db/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/suggestion/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/db_lib/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/log_lib/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/config/>")
endif()

if(build_coverage)
  set(CMAKE_BUILD_TYPE Debug)
  cmake_policy(SET CMP0175 OLD) # CodeCoverage has missing arguments still.
  include(CodeCoverage)
  file(GLOB_RECURSE COVERED_SRC_MODULES "${TIMESHEETING_DIR}/src/modules/*")
  file(GLOB_RECURSE COVERED_SRC_LIBS "${TIMESHEETING_DIR}/src/libs/*")
  add_executable(timesheeting_coverage
    "${COVERED_SRC_MODULES}" "${COVERED_SRC_LIBS}" "${TEST_SRC}")
  target_link_libraries(timesheeting_coverage
    PRIVATE Catch2::Catch2WithMain
    PUBLIC test_utils
    keys cli tui exporter db time_lib log_lib config)
  target_include_directories(timesheeting_coverage PUBLIC
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/test/test_utils/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/version/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/core/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/ui/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/exporter/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/db/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/time_lib/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/suggestion/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/db_lib/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/libs/log_lib/>"
    "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/src/modules/config/>")
  set_target_properties(timesheeting_coverage PROPERTIES COMPILE_FLAGS
    "${CMAKE_CXX_FLAGS} -O0 -fprofile-arcs -ftest-coverage -fno-default-inline -fno-elide-constructors")
  setup_target_for_coverage_lcov(
    NAME timesheeting_coverage_report
    EXECUTABLE timesheeting_coverage
    BASE_DIRECTORY "${TIMESHEETING_DIR}"
    LCOV_ARGS "--ignore-errors=inconsistent"
    EXCLUDE "${TIMESHEETING_DIR}/test/*" "/usr/*")
endif()
