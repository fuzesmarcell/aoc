cmake_minimum_required(VERSION 3.10)

project(AOC)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options("$<$<CONFIG:DEBUG>:/fsanitize=address>")

set (CMAKE_CXX_STANDARD 20)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    link_libraries(OpenMP::OpenMP_C)
endif()

add_library(aoc_lib STATIC "aoc.cpp" aoc.h "2022/day_01.cpp")

add_subdirectory(third_party)

add_subdirectory(2017)
add_subdirectory(2019)
add_subdirectory(2020)
add_subdirectory(2021)
add_subdirectory(2022)
add_subdirectory(2023)