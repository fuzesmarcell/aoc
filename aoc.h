/*
 * Advent of code common code
 *
 *
 * File      aoc.h
 * Author    Fuzes Marcell
 * Date      2021.12.11
 */

#ifndef AOC_H
#define AOC_H

#include <inttypes.h>
#include <stdbool.h>

#define COUNT_OF(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// color codes for printing
#define KRED   "\x1B[31m"
#define KGRN   "\x1B[32m"
#define KYEL   "\x1B[33m"
#define KBLU   "\x1B[34m"
#define KMAG   "\x1B[35m"
#define KCYN   "\x1B[36m"
#define KWHT   "\x1B[37m"
#define RESET  "\x1B[0m"

typedef int32_t bool32_t;

#endif // AOC_H