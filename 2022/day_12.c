#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_DIM 100

static int nx, ny;
static char map[MAX_DIM][MAX_DIM];

static bool vmap[MAX_DIM][MAX_DIM]; // visited nodes map

static int start_x, start_y;
static int end_x, end_y;

static inline char get_value(int x, int y);
static int traverse_map(int x, int y, char prev_c, int depth);

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
				map[ny][nx] = 'd'; // TODO: Switch back to 'z'
			}
			else {
				map[ny][nx] = c;
			}
		}
		++ny;
	}

	int px = end_x, py = end_y;

	traverse_map(px, py, 'd', 0);

	return 0;
}

char get_value(int x, int y) {
	if (x < 0 || x >= nx || y < 0 || y >= ny)
		return 'z'+1;
	return map[y][x];
}

int traverse_map(int x, int y, char prev_c, int depth) {
	if (x == start_x && y == start_y)
		return depth;

	char c = get_value(x, y);
	if (c != prev_c && c != prev_c - 1) {
		return INT_MAX;
	}

	// TODO: Instead of storing a normal map of only chars
	// we need to store the step numbers inside of them

	vmap[y][x] = true;

	int offsets[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	int steps[4] = { INT_MAX, INT_MAX, INT_MAX, INT_MAX };
	for (int i = 0; i < 4; i++) {
		int xo = offsets[i][0];
		int yo = offsets[i][1];

		int px = x + xo;
		int py = y + yo;
		if (px < 0 || px >= nx || py < 0 || py >= ny)
			continue;

		if (!vmap[py][px])
			steps[i] = traverse_map(px, py, c, depth+1);
	}

	vmap[y][x] = false;

	return depth-1;
}
