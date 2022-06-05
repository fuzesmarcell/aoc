cmake_minimum_required(VERSION 3.10)


set(prefix aoc_2019_)
add_executable(${prefix}01 day_01.c)
add_executable(${prefix}02 day_02.c)
add_executable(${prefix}03 day_03.c)
add_executable(${prefix}04 day_04.c)
add_executable(${prefix}05 day_05.c)
add_executable(${prefix}06 day_06.c)
add_executable(${prefix}07 day_07.c)
add_executable(${prefix}08 day_08.c)
add_executable(${prefix}09 day_09.c)
add_executable(${prefix}10 day_10.c)
add_executable(${prefix}11 day_11.c)
add_executable(${prefix}12 day_12.c)

add_executable(${prefix}13 day_13.c)
if (MSVC)
	target_link_options(${prefix}13 PRIVATE "/SUBSYSTEM:WINDOWS")
endif()

add_executable(${prefix}14 day_14.c)
add_executable(${prefix}15 day_15.c)