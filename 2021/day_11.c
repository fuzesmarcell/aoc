/*
 * Advent of code common code
 *
 *
 * File      day_11.c
 * Author    Fuzes Marcell
 * Date      2021.12.11
 */

#include <stdio.h>
#include <string.h>

#include <aoc.h>

#define W (10)
#define H (10)
#define N ((W) * (H))

struct
{
	union {
		int cells[W][H];
		int array[N];
	};

	// updated each tick weather octopus flashed already or not
	bool32_t flashed[W][H];
	
} octopuses;

static int synch_reference[N] = { 0 };

static void flash(int x, int y, int inc) {
	// Here I had a bounds check error but assan was not enabled.
	// After enabling assan catched the error!!
	if ((x < 0 || x >= W) || (y < 0 || y >= H)) {
		return;
	}
	
	octopuses.cells[y][x] += inc;
	int n = octopuses.cells[y][x];
	bool f = octopuses.flashed[y][x];

	if (n <= 9 || f) { return; }

	octopuses.flashed[y][x] = true;

	// left right
	flash(x + 1, y, 1);
	flash(x - 1, y, 1);
	
	// up down
	flash(x, y + 1, 1);
	flash(x, y - 1, 1);

	// diagonal
	flash(x + 1, y + 1, 1);
	flash(x - 1, y + 1, 1);
	flash(x - 1, y - 1, 1);
	flash(x + 1, y - 1, 1);

	return;
}

static void print_octopuses() {
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int n = octopuses.cells[y][x];
			if (n == 0) {
				printf(KRED "%d" RESET, n);
			}
			else {
				printf("%d", n);
			}
		}
		printf("\n");
	}
}

int main(void) {

	uint64_t part1 = 0;
	int cnt = 0;
	for (char c = getchar(); c > 0; c = getchar()) {
		if (c != '\n') { octopuses.array[cnt++] = c - '0'; }
	}

	for (int i = 0; ; i++) {
		// increase energy level
		for (int k = 0; k < COUNT_OF(octopuses.array); k++) {
			octopuses.array[k]++;
		}

		// flash
		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {
				flash(x, y, 0);
			}
		}

		// reset power levels
		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {
				int n = octopuses.cells[y][x];
				if (n > 9) { octopuses.cells[y][x] = 0; part1++; }
			}
		}

		printf("\r%d", i + 1);
		fflush(stdout);
		if (memcmp(octopuses.array, synch_reference, sizeof(synch_reference)) == 0) {
			printf("\nPart 02: %d\n", i + 1);
			break;
		}

#if 0
		printf("After step: %d\n", i + 1);
		print_octopuses();
		printf("\n");
#endif

		// reset flash table
		memset(octopuses.flashed, 0, sizeof(octopuses.flashed));
	}

	printf("Part 01: %llu\n", (long long unsigned int)part1);

	return 0;
}
