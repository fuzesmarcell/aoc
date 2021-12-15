/*
 * Advent of code
 *
 *
 * File      day_15.cpp
 * Author    Fuzes Marcell
 * Date      2021.12.15
 */

#include <stdio.h>

#include <array>
#include <unordered_set>

#include "../aoc.h"

#define INITIAL_WIDTH 100
#define INITIAL_HEIGHT 100

#define INITIAL_LENGTH (INITIAL_WIDTH * INITIAL_HEIGHT)

#define WIDTH (INITIAL_WIDTH * 5)
#define HEIGHT (INITIAL_HEIGHT * 5)

#define LENGTH (WIDTH * HEIGHT)

static int initial_grid[INITIAL_LENGTH];
static int grid[LENGTH];

static int dist[LENGTH];
static int prev[LENGTH];

int main(void) {

	int cnt = 0;
	for (char c = getchar(); c > 0; c = getchar()) {
		if (c == '\n') { continue; }
		initial_grid[cnt++] = c - '0';
	}

	for (int y = 0; y < INITIAL_HEIGHT; y++) {
		for (int x = 0; x < INITIAL_WIDTH; x++) {
			grid[x + y * WIDTH] = initial_grid[x + y * INITIAL_WIDTH];
		}
	}

	for (int yy = 0; yy < 5; yy++) {
		for (int xx = 0; xx < 5; xx++) {
			if (xx == 0 && yy == 0) { continue; }

			int *dest = &grid[(xx * INITIAL_WIDTH) + (yy * INITIAL_HEIGHT) * WIDTH];
			
			int *source = &grid[((xx - 1) * INITIAL_WIDTH) + (yy * INITIAL_HEIGHT) * WIDTH];
			if (yy >> 0) {
				source = &grid[(xx * INITIAL_WIDTH) + ((yy - 1) * INITIAL_HEIGHT) * WIDTH];
			}

			for (int y = 0; y < INITIAL_HEIGHT; y++) {
				for (int x = 0; x < INITIAL_WIDTH; x++) {
					int n = source[x + y * WIDTH] + 1;
					dest[x + y * WIDTH] = n > 9 ? 1 : n;
				}
			}
		}
	}

#if 0
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			printf("%d", grid[x + y * WIDTH]);
		}
		printf("\n");
	}
#endif

	std::unordered_set<int> q;
	q.reserve(LENGTH);

	// top left corner
	int source = 0;

	for (int i = 0; i < LENGTH; i++) {
		q.insert(i);
		dist[i] = INT_MAX;
		prev[i] = -1;
	}

	dist[0] = 0;

	while (!q.empty()) {

		uint64_t size = q.size();
		if (size % 100 == 0) {
			printf("\r%lld", size);
			fflush(stdout);
		}

		// find minimum value
		int min = INT_MAX;
		int u = -1;
		for (int k : q) {
			int distance = dist[k];
			if (distance < min) {
				min = distance;
				u = k;
			}
		}

		// mark as processed
		q.erase(u);

		int y = u / HEIGHT;
		int x = u - (WIDTH * y);

		int neighbours[4][2] = {
			{x + 1, y},
			{x - 1, y},
			{x, y + 1},
			{x, y - 1},
		};

		for (int i = 0; i < 4; i++) {
			int xx = neighbours[i][0];
			int yy = neighbours[i][1];
			if (xx < 0 || xx >= WIDTH ||
				yy < 0 || yy >= HEIGHT)
			{
				continue;
			}

			int v = xx + yy * WIDTH;

			if (q.count(v)) {
				// int len = abs(grid[u] - grid[v]);
				int len = grid[v];
				int alt = dist[u] + len;
				if (alt < dist[v]) {
					dist[v] = alt;
					prev[v] = u;
				}
			}
		}
	}

	std::vector<int> s;
	int u = (LENGTH - 1); // target
	if (prev[u] != -1 || u == source) {
		while (u != -1) {
			s.insert(s.begin(), u);
			u = prev[u];
		}
	}

	int64_t score = 0;
	for (int i : s) {
		if (i == 0) { continue; }
		score += grid[i];
	}

	printf("\n%lld\n", score);

#if 0
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			bool inside = false;
			int key = x + y * WIDTH;
			for (int k : s) {
				if (k == key) {
					inside = true;
					break;
				}
			}

			if (inside) {
				printf(KRED "%d" RESET, grid[key]);
			}
			else {
				printf("%d", grid[key]);
			}
		}
		printf("\n");
	}
#endif
	return 0;
}