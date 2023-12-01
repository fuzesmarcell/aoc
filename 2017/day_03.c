#include <stdio.h>
#include <math.h>
#include <assert.h>

#define DIM 2048
#define DIM_HALF (DIM/2)

static int grid[DIM][DIM];

static int sum_adjacent(int x, int y);
static int get_value(int x, int y);
static void set_value(int x, int y, int v);

int main(int argc, char** argv) {
	int x = 1, y = -1;
	int step = 2;

	set_value(0, 0, 1);
	set_value(1, 0, 1);
	set_value(1, -1, 2);

	int square_id = 312051;
	int square = 0;

	while (1) {
		for (int i = 0; i < step; i++) {
			--x;
			square = sum_adjacent(x, y);
			if (square > square_id) goto end;
			set_value(x, y, square);
		}

		for (int i = 0; i < step; i++) {
			++y;
			square = sum_adjacent(x, y);
			if (square > square_id) goto end;
			set_value(x, y, square);
		}

		++step;
		for (int i = 0; i < step; i++) {
			++x;
			square = sum_adjacent(x, y);
			if (square > square_id) goto end;
			set_value(x, y, square);
		}

		for (int i = 0; i < step; i++) {
			--y;
			square = sum_adjacent(x, y);
			if (square > square_id) goto end;
			set_value(x, y, square);
		}

		++step;
	}

end:;

	printf("%d\n", square);

	return 0;
}

int sum_adjacent(int x, int y) {
	int result = 0;
	result += get_value(x + 1, y);
	result += get_value(x - 1, y);
	result += get_value(x, y - 1);
	result += get_value(x, y + 1);
	result += get_value(x + 1, y + 1);
	result += get_value(x - 1, y - 1);
	result += get_value(x + 1, y - 1);
	result += get_value(x - 1, y + 1);

	return result;
}

int get_value(int x, int y) {
	x += DIM_HALF;
	y += DIM_HALF;

	if (x < 0 || x >= DIM ||
		y < 0 || y >= DIM) {
		return 0;
	}

	return grid[y][x];
}

void set_value(int x, int y, int v) {
	x += DIM_HALF;
	y += DIM_HALF;

	if (x < 0 || x >= DIM ||
		y < 0 || y >= DIM) {
		assert(!"out of bounds");
		return;
	}

	grid[y][x] = v;
}