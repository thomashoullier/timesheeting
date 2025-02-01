cmake_minimum_required(VERSION 3.20)

project(timesheeting)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_FLAGS "-Wall -O2 -Wextra -Wformat=2 -Wpedantic -march=native")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For development with CLANGD

set(TIMESHEETING_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Dependencies
## Manually set the ncurses libraries
if (EXISTS "/usr/lib/libncurses.so")
  set(CURSES_LIBRARIES "/usr/lib/libncurses.so;/usr/lib/libtinfo.so")
else ()
  set(CURSES_LIBRARIES "/usr/lib64/libncurses.so;/usr/lib64/libtinfo.so")
endif()

# Main
file(GLOB_RECURSE SRC "${TIMESHEETING_DIR}/src/*.cpp")
add_executable(timesheeting "${TIMESHEETING_DIR}/src/main.cpp" ${SRC})
target_link_libraries(timesheeting PUBLIC ${CURSES_LIBRARIES})
