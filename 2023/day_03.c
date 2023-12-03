#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
	int ndigits;
	char digits[3];
	int positions[3][2];
} PartNumber;

#define MAX_DIM 512
static char schematic[MAX_DIM][MAX_DIM];
static int nx, ny;

static PartNumber parts[MAX_DIM*MAX_DIM];
static int nparts;

static void part_add(PartNumber* part, char c, int x, int y);
static int part_to_number(PartNumber* part);

static inline bool is_symbol(int x, int y);
static inline bool has_adjacent_symbol(int x, int y);

static int lookup_part(int x, int y);
static int find_gear_ratio(int x, int y);

int main() {

	char buffer[512];
	char* line;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		nx = 0;
		size_t len = strlen(line)-1;
		size_t i = 0;
		while (i < len) {
			if (isdigit(line[i])) {
				PartNumber* part = &parts[nparts++];

				char c;
				do {
					c = line[i];
					part_add(part, c, nx, ny);
					schematic[ny][nx++] = c;
					++i;
				} while (i < len && isdigit(line[i]));
			}
			else {
				schematic[ny][nx++] = line[i++];
			}
		}

		++ny;
	}

	assert(nx < MAX_DIM);
	assert(ny < MAX_DIM);

	size_t result = 0;
	for (size_t i = 0; i < nparts; i++) {
		PartNumber* part = &parts[i];

		int number = 0;
		for (size_t j = 0; j < part->ndigits; j++) {
			if (has_adjacent_symbol(part->positions[j][1],
									part->positions[j][0])) {
				number = part_to_number(part);
				result += number;
				break;
			}
		}
	}

	size_t result1 = 0;
	for (int y = 0; y < ny; y++) {
		for (int x = 0; x < nx; x++) {
			char c = schematic[y][x];
			if (c == '*')
				result1 += find_gear_ratio(x, y);
		}
	}

	return 0;
}

int part_to_number(PartNumber* part) {
	int n = 0;
	int base = 1;
	for (int i = part->ndigits - 1; i >= 0; i--) {
		char c = part->digits[i];
		n += (c - '0')*base;
		base *= 10;
	}

	return n;
}

void part_add(PartNumber* part, char c, int x, int y) {
	part->digits[part->ndigits] = c;
	part->positions[part->ndigits][0] = y;
	part->positions[part->ndigits][1] = x;
	++part->ndigits;
}

bool is_symbol(int x, int y) {
	if (x < 0 || x >= nx ||
		y < 0 || y >= ny)
		return false;

	char c = schematic[y][x];

	if (c == '.')
		return false;
	if (isdigit(c))
		return false;
	return true;
}

bool has_adjacent_symbol(int x, int y) {
	if (is_symbol(x + 1, y)) return true;
	if (is_symbol(x - 1, y)) return true;
	if (is_symbol(x, y + 1)) return true;
	if (is_symbol(x, y - 1)) return true;
	if (is_symbol(x + 1, y + 1)) return true;
	if (is_symbol(x - 1, y - 1)) return true;
	if (is_symbol(x + 1, y - 1)) return true;
	if (is_symbol(x - 1, y + 1)) return true;

	return false;
}

int lookup_part(int x, int y) {
	for (int i = 0; i < nparts; i++) {
		PartNumber* part = &parts[i];
		for (int j = 0; j < part->ndigits; j++) {
			int py = part->positions[j][0];
			int px = part->positions[j][1];

			if (x == px && y == py) {
				return i;
			}
		}
	}

	return -1;
}

static bool has_id(int* ids, int len, int id) {
	for (int i = 0; i < len; i++) {
		if (ids[i] == id) {
			return true;
		}
	}
	return false;
}

int find_gear_ratio(int x, int y) {
	int npart_ids = 0;
	int part_ids[8] = { 0 };

	int offsets[8][2] = {
		{ 1,  0},
		{-1,  0},

		{ 0,  1},
		{ 0, -1},

		{ 1,  1},
		{-1, -1},

		{ 1, -1},
		{-1,  1},
	};

	for (int i = 0; i < 8; i++) {
		int xo = offsets[i][0];
		int yo = offsets[i][1];

		int id;
		if (id = lookup_part(x + xo, y + yo), id >= 0 &&
			!has_id(part_ids, npart_ids, id)) {
			part_ids[npart_ids++] = id;
		}
	}

	if (npart_ids == 2) {
		int ratio = part_to_number(&parts[part_ids[0]]) *
			part_to_number(&parts[part_ids[1]]);
		return ratio;
	}

	return 0;
}
