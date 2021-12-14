/*
 * Advent of code
 *
 *
 * File      day_13.c
 * Author    Fuzes Marcell
 * Date      2021.12.13
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <aoc.h>

#define DOT_INITIAL_COUNT (4096)

typedef struct {
	bool is_free;
	int32_t x, y;
} dot_t;

typedef struct {
	// capacity of the dots arary
	int cap;
	// valid length of items either free or not
	int len;
	dot_t *dots;
} grid_t;

// does not allow duplicated dots!
static dot_t *grid_add(grid_t *grid, int32_t x, int32_t y) {
	// check for duplicates
	for (int i = 0; i < grid->len; i++) {
		dot_t *dot = &grid->dots[i];
		if (dot->is_free) {
			continue;
		}

		if (dot->x == x && dot->y == y) {
			return dot;
		}
	}

	// check for free items
	for (int i = 0; i < grid->len; i++) {
		dot_t *dot = &grid->dots[i];
		if (dot->is_free) {
			dot->is_free = false;
			dot->x = x;
			dot->y = y;
			return dot;
		}
	}

	// push to end of buffer or realloc if needed!
	dot_t *dot = NULL;
	if (grid->len + 1 <= grid->cap) {
		dot = &grid->dots[grid->len++];
	}
	else {
		grid->cap *= 2;
		dot_t *temp = realloc(grid->dots, grid->cap * sizeof(dot_t));
		assert(temp != NULL);
		if (temp == NULL) {
			free(grid->dots);
			return NULL;
		}

		grid->dots = temp;
		dot = &grid->dots[grid->len++];
	}

	assert(dot != NULL);
	if (dot == NULL) {
		return NULL;
	}

	dot->is_free = false;
	dot->x = x;
	dot->y = y;

	return dot;
}

static dot_t *grid_get(grid_t *grid, int x, int y) {
	for (int i = 0; i < grid->len; i++) {
		dot_t *dot = &grid->dots[i];
		if (dot->is_free) {
			continue;
		}

		if (dot->x == x && dot->y == y) {
			return dot;
		}
	}

	return NULL;
}

static void grid_remove(grid_t *grid, int x, int y) {
	dot_t *dot = grid_get(grid, x, y);
	assert(dot);
	if (dot) {
		dot->is_free = true;
		dot->x = 0;
		dot->y = 0;
	}
}

static int grid_count(grid_t *grid) {
	int result = 0;
	for (int i = 0; i < grid->len; i++) {
		if (!grid->dots[i].is_free) {
			result++;
		}
	}

	return result;
}

static int fold_y(grid_t *grid, int fold_y) {
	for (int i = 0; i < grid->len; i++) {
		dot_t *dot = &grid->dots[i];
		if (dot->is_free) {
			continue;
		}

		// dots will never appear exactly on a fold line
		assert(dot->y != fold_y);

		if (dot->y > fold_y) {
			int y = (dot->y - fold_y);
			y = -y;
			y += fold_y;

			grid_add(grid, dot->x, y);
			grid_remove(grid, dot->x, dot->y);
		}
	}

	return grid_count(grid);
}

static int fold_x(grid_t* grid, int fold_x) {
	for (int i = 0; i < grid->len; i++) {
		dot_t* dot = &grid->dots[i];
		if (dot->is_free) {
			continue;
		}

		// dots will never appear exactly on a fold line
		assert(dot->x != fold_x);

		if (dot->x > fold_x) {
			int x = (dot->x - fold_x);
			x = -x;
			x += fold_x;

			grid_add(grid, x, dot->y);
			grid_remove(grid, dot->x, dot->y);
		}
	}

	return grid_count(grid);
}

int main(void) {

	grid_t grid = {
		.cap = DOT_INITIAL_COUNT,
	};

	grid.dots = calloc(grid.cap, sizeof(dot_t));

	char buffer[128];
	while (fgets(buffer, sizeof(buffer), stdin)) {
		int x, y;
		int args_read = sscanf(buffer, "%d,%d", &x, &y);
		if (args_read != 2) {
			break;
		}

		grid_add(&grid, x, y);
	}

	while (fgets(buffer, sizeof(buffer), stdin)) {
		int fold = 0;
		if (sscanf(buffer, "fold along y=%d", &fold) == 1) {
			fold_y(&grid, fold);
		}
		else if (sscanf(buffer, "fold along x=%d", &fold) == 1) {
			fold_x(&grid, fold);
		}
	}

	for (int y = 0; y < 50; y++) {
		for (int x = 0; x < 50; x++) {
			dot_t *dot = grid_get(&grid, x, y);
			if (dot == NULL) {
				printf(".");
			}
			else {
				printf(KGRN "#" RESET);
			}
		}

		printf("\n");
	}

		return 0;
}