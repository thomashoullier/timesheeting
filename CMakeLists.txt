cmake_minimum_required(VERSION 3.20)

project(timesheeting)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For development with CLANGD

set(TIMESHEETING_DIR "${CMAKE_CURRENT_LIST_DIR}")
file(GLOB_RECURSE TIMESHEETING_SRC "${TIMESHEETING_DIR}/src/*.cpp")

# Dependencies
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
find_library(FORM_LIBRARY form)

find_package (SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

# Main
add_executable(timesheeting_main "${TIMESHEETING_DIR}/src/main.cpp"
  "${TIMESHEETING_SRC}")
target_link_libraries(timesheeting_main ${CURSES_LIBRARIES} ${FORM_LIBRARY}
                      ${SQLite3_LIBRARIES})
set_target_properties(timesheeting_main PROPERTIES COMPILE_FLAGS
  "${CMAKE_CXX_FLAGS} -g -Wall -O2 -Wextra -march=native")
