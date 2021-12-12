/*
 * Advent of code
 *
 *
 * File      day_17.c
 * Author    Fuzes Marcell
 * Date      2021.12.12
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <aoc.h>

typedef struct {
	bool active;
	int32_t x, y, z;
} cube_t;

typedef struct {
	int capacity;
	int count;
	cube_t *cubes;

	// The border where cubes are still active
	int32_t border_x;
	int32_t border_y;
	int32_t border_z;

} grid_t;

static bool grid_alloc(int len, grid_t *grid) {
	grid->count = 0;
	grid->capacity = len;
	grid->border_x = 0;
	grid->border_y = 0;
	grid->border_z = 0;

	grid->cubes = calloc(grid->capacity, sizeof(cube_t));
	if (grid->cubes == NULL) {
		return false;
	}

	return true;
}

static cube_t *grid_get(const grid_t *grid, int x, int y, int z) {
	for (int i = 0; i < grid->count; i++) {
		cube_t *cube = &grid->cubes[i];
		if (cube->x == x && cube->y == y && cube->z == z) {
			return cube;
		}
	}

	return NULL;
}

static void grid_set(grid_t *grid, int x, int y, int z, bool active) {
	cube_t *cube = grid_get(grid, x, y, z);
	if (cube == NULL) {
		assert((grid->count + 1) < grid->capacity);
		cube = &grid->cubes[grid->count++];
	}

	cube->x = x;
	cube->y = y;
	cube->z = z;
	cube->active = active;
}

int main(void) {

	grid_t a, b;
	if (!grid_alloc(4096, &a) ||
		!grid_alloc(4096, &b)) {
		fprintf(stderr, "Unexpected NULL pointer!\n");
		return -1;
	}

	// read input
	{
		int x = 0, y = 0;
		for (char c = getchar(); c > 0; c = getchar()) {
			if (c != '\n') {
				grid_set(&a, x, y, 0, c == '#' ? true : false);
				x++;
			}
			else {
				y++;
			}
		}
	}

	return 0;
}