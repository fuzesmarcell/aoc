cmake_minimum_required(VERSION 3.10)

add_library(aoc_2019_lib 
  shared_advent.h
  shared_advent.c
)

if (!MSVC)
	target_link_libraries(aoc_2019_lib m)
endif()

add_library(intcode_lib 
  intcode.h
  intcode.c
)

set(prefix aoc_2019_)
add_executable(${prefix}01 day_01.c)
add_executable(${prefix}02 day_02.c)
add_executable(${prefix}03 day_03.c)
add_executable(${prefix}04 day_04.c)
add_executable(${prefix}05 day_05.c)
add_executable(${prefix}06 day_06.c)
target_link_libraries(${prefix}06 aoc_2019_lib)
add_executable(${prefix}07 day_07.c)
add_executable(${prefix}08 day_08.c)
target_link_libraries(${prefix}08 aoc_2019_lib)
add_executable(${prefix}09 day_09.c)
target_link_libraries(${prefix}09 intcode_lib)
add_executable(${prefix}10 day_10.c)
target_link_libraries(${prefix}10 aoc_2019_lib)
add_executable(${prefix}11 day_11.c)
target_link_libraries(${prefix}11 aoc_2019_lib intcode_lib)
add_executable(${prefix}12 day_12.c)
target_link_libraries(${prefix}12 aoc_2019_lib)

add_executable(${prefix}13 day_13.c)
target_link_libraries(${prefix}13 raylib intcode_lib)

add_executable(${prefix}14 day_14.c)
target_link_libraries(${prefix}14 aoc_2019_lib)
add_executable(${prefix}15 day_15.c)
target_link_libraries(${prefix}15 intcode_lib)
