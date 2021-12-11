cmake_minimum_required(VERSION 3.10)

project(AOC)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options("$<$<CONFIG:DEBUG>:/fsanitize=address>")

add_library(aoc_lib STATIC aoc.c aoc.h)

add_subdirectory(2019)
add_subdirectory(2020)
add_subdirectory(2021)