cmake_minimum_required(VERSION 3.20)

project(timesheeting)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For development with CLANGD

set(TIMESHEETING_DIR "${CMAKE_CURRENT_LIST_DIR}")
file(GLOB_RECURSE TIMESHEETING_SRC "${TIMESHEETING_DIR}/src/*.cpp")

# Main
add_executable(timesheeting_main "${TIMESHEETING_DIR}/src/main.cpp"
  "${TIMESHEETING_SRC}")
set_target_properties(timesheeting_main PROPERTIES COMPILE_FLAGS
  "${CMAKE_CXX_FLAGS} -g -Wall -O2 -Wextra -march=native")
