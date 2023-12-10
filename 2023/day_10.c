#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define MAX_GRID 500

static char grid[MAX_GRID][MAX_GRID];
static int nx, ny;

static bool vgrid[MAX_GRID][MAX_GRID];

typedef struct {
	int x, y;
} int2;

static int2 conn_off[][2] = {
	['|'] = {{0, -1},{ 0,  1}},
	['-'] = {{1,  0},{-1,  0}},
	['L'] = {{0, -1},{ 1,  0}},
	['J'] = {{0, -1},{-1,  0}},
	['7'] = {{0,  1},{-1,  0}},
	['F'] = {{0,  1},{ 1,  0}},
};

static char get_value(int x, int y);
static bool int2_eq(int2 a, int2 b) {
	return a.x == b.x && a.y == b.y;
}

typedef struct {
	int x, y;
	int prev_x, prev_y;
	int end_x, end_y;
} PipeIt;

static PipeIt pipe_it_make(int x, int y, int end_x, int end_y);
static bool pipe_it_next(PipeIt* it);
static int dist_from_start(int x, int y, int start_x, int start_y);

int main() {
	char buffer[512];
	char* line;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		nx = 0;
		size_t len = strlen(line)-1;
		for (size_t i = 0; i < len; i++) {
			grid[ny][nx++] = line[i];
		}
		++ny;
	}

	int start_x, start_y;
	for (int y = 0; y < ny; y++) {
		for (int x = 0; x < nx; x++) {
			if (grid[y][x] == 'S') {
				start_x = x;
				start_y = y;
				goto start_found;
			}
		}
	}
start_found:;

	int2 offsets[] = {
		{ 0,  1},
		{ 0, -1},
		{ 1,  0},
		{-1,  0},
	};

	int ncan = 0;
	int2 can_connect[2];
	for (int i = 0; i < 4; i++) {
		int2 o = offsets[i];
		int2 next_p = { start_x + o.x, start_y + o.y };
		char c = get_value(next_p.x, next_p.y);
		if (c == '.') continue;

		int2 p[2];
		p[0] = conn_off[c][0];
		p[1] = conn_off[c][1];

		for (int j = 0; j < 2; j++) {
			int2 np = { next_p.x + p[j].x, next_p.y + p[j].y };
			if (np.x == start_x && np.y == start_y) {
				int2 cc = { p[j].x*-1, p[j].y*-1 };
				can_connect[ncan++] = cc;
				break;
			}
		}
	};

	assert(ncan == 2);

	char pipes[6] = { '|', '-', 'L', 'J', '7', 'F' };

	char s_pipe_type = '?';
	for (int i = 0; i < 6; i++) {
		int2 p[2];
		p[0] = conn_off[pipes[i]][0];
		p[1] = conn_off[pipes[i]][1];

		bool eq0 = int2_eq(p[0], can_connect[0]);
		bool eq1 = int2_eq(p[1], can_connect[1]);

		bool eq2 = int2_eq(p[0], can_connect[1]);
		bool eq3 = int2_eq(p[1], can_connect[0]);

		if ((eq0 && eq1) ||
			(eq2 && eq3)) {
			s_pipe_type = pipes[i];
			break;
		}
	}

	assert(s_pipe_type != '?');

	grid[start_y][start_x] = s_pipe_type;

	int2 s_o[2];
	s_o[0] = conn_off[s_pipe_type][0];

	PipeIt it;
	it.prev_x = start_x;
	it.prev_y = start_y;
	it.x = start_x+s_o[0].x;
	it.y = start_y+s_o[0].y;
	it.end_x = start_x;
	it.end_y = start_y;

	int dist = INT_MIN;
	do {
		int d = dist_from_start(it.x, it.y, start_x, start_y);
		if (d > dist)
			dist = d;
	} while (pipe_it_next(&it));

	printf("Result %d\n", dist);

	return 0;
}

char get_value(int x, int y) {
	if (x < 0 || x >= nx || y < 0 || y >= ny) {
		return '.';
	}
	return grid[y][x];
}

PipeIt pipe_it_make(int x, int y, int end_x, int end_y) {
	PipeIt it;
	it.x = x;
	it.y = y;
	it.end_x = end_x;
	it.end_y = end_y;
	it.prev_x = -1;
	it.prev_y = -1;
}

bool pipe_it_next(PipeIt* it) {
	char c = get_value(it->x, it->y);
	assert(c != '.');

	int2 po[2];
	po[0] = conn_off[c][0];
	po[1] = conn_off[c][1];

	int x0 = it->x + po[0].x;
	int y0 = it->y + po[0].y;

	int x1 = it->x + po[1].x;
	int y1 = it->y + po[1].y;

	if (x0 == it->prev_x && y0 == it->prev_y) {
		it->prev_x = it->x;
		it->prev_y = it->y;
		it->x = x1;
		it->y = y1;
	}
	else {
		it->prev_x = it->x;
		it->prev_y = it->y;
		it->x = x0;
		it->y = y0;
	}

	if (it->x == it->end_x && it->y == it->end_y) {
		return false;
	}

	return true;
}

int dist_from_start(int x, int y, int start_x, int start_y) {
	int result = INT_MAX;

	char c = get_value(x, y);
	assert(c != '.');

	int2 off[2];
	off[0] = conn_off[c][0];
	off[1] = conn_off[c][1];

	PipeIt it0;
	it0.end_x = start_x;
	it0.end_y = start_y;

	it0.prev_x = x + off[0].x;
	it0.prev_y = y + off[0].y;

	it0.x = x;
	it0.y = y;

	int dist0 = 0;
	do {
		++dist0;
	} while (pipe_it_next(&it0));

	PipeIt it1;
	it1.end_x = start_x;
	it1.end_y = start_y;

	it1.prev_x = x + off[1].x;
	it1.prev_y = y + off[1].y;

	it1.x = x;
	it1.y = y;

	int dist1 = 0;
	do {
		++dist1;
	} while (pipe_it_next(&it1));

	if (dist1 < dist0)
		return dist1;
	return dist0;
}
