cmake_minimum_required(VERSION 3.20)

project(timesheeting)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For development with CLANGD

set(TIMESHEETING_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Dependencies
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
find_library(MENU_LIBRARY menu)

find_package (SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

find_package(tomlplusplus REQUIRED)

include_directories(BEFORE /usr/include/tclap)

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
target_include_directories(db_lib PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/libs>")

# Internal modules
file(GLOB_RECURSE KEYS_SRC "${TIMESHEETING_DIR}/modules/ui/keys/*.cpp")
add_library(keys STATIC "${KEYS_SRC}")
target_include_directories(keys PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

file(GLOB_RECURSE CORE_SRC "${TIMESHEETING_DIR}/modules/core/*.cpp")
add_library(core STATIC "${CORE_SRC}")
target_link_libraries(core time_lib)
target_include_directories(core PUBLIC
  "$<BUILD_INTERFACE:${TIMESHEETING_DIR}/modules>")

# Main
file(GLOB_RECURSE TIMESHEETING_SRC "${TIMESHEETING_DIR}/src/*.cpp")
add_executable(timesheeting_main "${TIMESHEETING_DIR}/src/main.cpp"
  "${TIMESHEETING_SRC}")
target_link_libraries(timesheeting_main
                      time_lib db_lib log_lib
                      keys core
                      ${CURSES_LIBRARIES} ${MENU_LIBRARY}
                      ${SQLite3_LIBRARIES} tomlplusplus::tomlplusplus
                      rapidfuzz::rapidfuzz)
set_target_properties(timesheeting_main PROPERTIES COMPILE_FLAGS
  "${CMAKE_CXX_FLAGS} -g -Wall -O2 -Wextra -march=native")
