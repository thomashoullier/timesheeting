cmake_minimum_required(VERSION 3.20)

project(timesheeting)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For development with CLANGD

set(TIMESHEETING_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Dependencies
find_package(Curses REQUIRED)
find_library(MENU_LIBRARY menu)
find_package (SQLite3)
find_package(tomlplusplus REQUIRED)
find_package(rapidfuzz REQUIRED)

# Internal libraries
file(GLOB_RECURSE TIME_LIB_SRC "${TIMESHEETING_DIR}/libs/time_lib/*.cpp")
add_library(time_lib STATIC "${TIME_LIB_SRC}")
target_include_directories(time_lib PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/libs>")

file(GLOB_RECURSE LOG_LIB_SRC "${TIMESHEETING_DIR}/libs/log_lib/*.cpp")
add_library(log_lib STATIC "${LOG_LIB_SRC}")
target_link_libraries(log_lib PRIVATE time_lib)
target_include_directories(log_lib PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/libs>")

file(GLOB_RECURSE DB_LIB_SRC "${TIMESHEETING_DIR}/libs/db_lib/*.cpp")
add_library(db_lib STATIC "${DB_LIB_SRC}")
target_link_libraries(db_lib PRIVATE ${SQLite3_LIBRARIES})
target_include_directories(db_lib PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/libs>")

file(GLOB_RECURSE NCURSES_LIB_SRC "${TIMESHEETING_DIR}/libs/ncurses_lib/*.cpp")
add_library(ncurses_lib STATIC "${NCURSES_LIB_SRC}")
target_link_libraries(ncurses_lib PRIVATE log_lib
                      ${CURSES_LIBRARIES} ${MENU_LIBRARY})
target_include_directories(ncurses_lib PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/libs>")

file(GLOB_RECURSE SUGGESTION_SRC "${TIMESHEETING_DIR}/libs/suggestion/*.cpp")
add_library(suggestion STATIC "${SUGGESTION_SRC}")
target_link_libraries(suggestion PRIVATE rapidfuzz::rapidfuzz)
target_include_directories(suggestion PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/libs>")

# Internal modules
add_library(version INTERFACE)
target_include_directories(version INTERFACE
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

file(GLOB_RECURSE KEYS_SRC "${TIMESHEETING_DIR}/modules/ui/keys/*.cpp")
add_library(keys STATIC "${KEYS_SRC}")
target_include_directories(keys PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

file(GLOB_RECURSE CORE_SRC "${TIMESHEETING_DIR}/modules/core/*.cpp")
add_library(core STATIC "${CORE_SRC}")
target_link_libraries(core PRIVATE time_lib ncurses_lib)
target_include_directories(core PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

file(GLOB_RECURSE DB_SRC "${TIMESHEETING_DIR}/modules/db/*.cpp")
add_library(db STATIC "${DB_SRC}")
target_link_libraries(db PRIVATE version db_lib time_lib core)
target_include_directories(db PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

file(GLOB_RECURSE CONFIG_SRC "${TIMESHEETING_DIR}/modules/config/*.cpp")
add_library(config STATIC "${CONFIG_SRC}")
target_link_libraries(config PRIVATE tomlplusplus::tomlplusplus)
target_include_directories(config PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

file(GLOB_RECURSE EXPORTER_SRC "${TIMESHEETING_DIR}/modules/exporter/*.cpp")
add_library(exporter STATIC "${EXPORTER_SRC}")
target_link_libraries(exporter PRIVATE time_lib db version)
target_include_directories(exporter PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

file(GLOB_RECURSE CLI_SRC "${TIMESHEETING_DIR}/modules/ui/cli/*.cpp")
add_library(cli STATIC "${CLI_SRC}")
target_include_directories(cli PRIVATE /usr/include/tclap)
target_link_libraries(cli PRIVATE version)
target_include_directories(cli PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

# Main
file(GLOB_RECURSE TIMESHEETING_SRC "${TIMESHEETING_DIR}/src/*.cpp")
add_executable(timesheeting_main "${TIMESHEETING_DIR}/src/main.cpp"
  "${TIMESHEETING_SRC}")
target_link_libraries(timesheeting_main PRIVATE
                      time_lib log_lib
                      keys core db config exporter cli
                      suggestion # TODO move to tui
                      )
set_target_properties(timesheeting_main PROPERTIES COMPILE_FLAGS
  "${CMAKE_CXX_FLAGS} -g -Wall -O2 -Wextra -march=native")
