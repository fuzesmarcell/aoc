#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_DIM 100

static int nx, ny;
static char map[MAX_DIM][MAX_DIM];

static int start_x, start_y;
static int end_x, end_y;

static inline char get_value(int x, int y);
static inline void draw_map(int px, int py);

static int traverse_map(int x, int y);

int main() {
	char buffer[512];
	char* line;

	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		nx = 0;
		size_t size = strlen(line)-1;
		for (size_t i = 0; i < size; i++, nx++) {
			char c = line[i];
			if (c == 'S') {
				start_x = nx;
				start_y = ny;
				map[ny][nx] = 'a';
			}
			else if (c == 'E') {
				end_x = nx;
				end_y = ny;
				map[ny][nx] = 'z';
			}
			else {
				map[ny][nx] = c;
			}
		}
		++ny;
	}

#if 0
	int px = end_x, py = end_y;
	int prev_x = px, prev_y = py;

	size_t steps = 0;
	while (!(px == start_x && py == start_y)) {

		draw_map(px, py);

		char c = get_value(px, py);
		int offsets[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

		int next_x = -1, next_y = -1;
		char min = 127;
		for (int i = 0; i < 4; i++) {
			int ox = offsets[i][0];
			int oy = offsets[i][1];
			char v = get_value(px+ox, py+oy);

			if (px+ox == prev_x && py+oy == prev_y)
				continue;

			bool is_valid = (v == c || v == c-1);
			if (is_valid && v < min) {
				min = v;
				next_x = px+ox;
				next_y = py+oy;
			}
		}

		assert(c == min || c-1 == min);

		++steps;
		prev_x = px;
		prev_y = py;
		px = next_x;
		py = next_y;
	}
#endif // 0

	return 0;
}

char get_value(int x, int y) {
	if (x < 0 || x >= nx || y < 0 || y >= ny)
		return 'z'+1;
	return map[y][x];
}

void draw_map(int px, int py) {
	for (int y = 0; y < ny; y++) {
		for (int x = 0; x < nx; x++) {
			if (x == px && y == py) {
				printf("@");
			}
			else {
				printf("%c", map[y][x]);
			}
		}
		printf("\n");
	}
	printf("\n\n");
}

int traverse_map(int x, int y, char prev_c) {
	char c = get_value(x, y);
	if (c != prev_c || c == prev_c - 1)
		return 0;

	int steps[4];

	steps[0] = traverse_map(x - 1, y, c);
	steps[1] = traverse_map(x + 1, y, c);
	steps[2] = traverse_map(x, y + 1, c);
	steps[3] = traverse_map(x, y - 1, c);

	int min = INT_MAX;
	for (int i = 0; i < 4; i++) {
		if (steps[i] < min) {
			min = steps[i];
		}
	}

	return min;
}
