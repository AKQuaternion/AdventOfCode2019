cmake_minimum_required(VERSION 3.15)
project(AdventOfCode2019)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(AdventOfCode2019 main.cpp day1.cpp day2.cpp daynnn.cpp Intcode.cpp Intcode.hpp day2.cpp day3.cpp day4.cpp day5.cpp day6.cpp day7.cpp day8.cpp day9.cpp runIntCode..cpp day10.cpp day11.cpp day12.cpp day13.cpp day14.cpp day15.cpp day16.cpp day17.cpp day18.cpp day19.cpp day20.cpp day21.cpp day22.cpp)
target_link_libraries(AdventOfCode2019 ${CURSES_LIBRARIES})
