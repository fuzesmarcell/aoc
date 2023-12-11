#include <stdio.h>
#include <string.h>
#include <math.h>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

typedef struct {
	int64_t x, y;
} Pixel;

std::vector<Pixel> pixels;
static int nx, ny;

static char get_value(int x, int y);

struct int2 {
	int64_t x, y;
};

int main() {
	char buffer[256];
	char* line;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		int len = (int)strlen(line)-1;
		nx = 0;
		for (int i = 0; i < len; i++, ++nx) {
			char c = line[i];
			if (c == '#')
				pixels.push_back({ nx, ny });
		}
		++ny;
	}

	std::vector<int> rows_to_expand;

	for (int y = 0; y < ny; y++) {
		bool row_empty = true;
		for (int x = 0; x < nx; x++) {
			if (get_value(x, y) == '#') {
				row_empty = false;
				break;
			}
		}
		if (row_empty)
			rows_to_expand.push_back(y);
	}

	std::vector<int> cols_to_expand;
	for (int x = 0; x < nx; x++) {
		bool col_empty = true;
		for (int y = 0; y < ny; y++) {
			if (get_value(x, y) == '#') {
				col_empty = false;
				break;
			}
		}
		if (col_empty)
			cols_to_expand.push_back(x);
	}

	std::unordered_map<int, int> to_expand_in_y;
	for (auto row : rows_to_expand) {
		for (size_t i = 0; i < pixels.size(); i++) {
			if (pixels[i].y > row) {
				++to_expand_in_y[i];
			}
		}
	}

	std::unordered_map<int, int> to_expand_in_x;
	for (auto col : cols_to_expand) {
		for (size_t i = 0; i < pixels.size(); i++) {
			if (pixels[i].x > col) {
				++to_expand_in_x[i];
			}
		}
	}

	for (auto [idx, add_y] : to_expand_in_y) {
		pixels[idx].y += (int64_t)add_y*(1'000'000llu-1);
	}

	for (auto [idx, add_x] : to_expand_in_x) {
		pixels[idx].x += (int64_t)add_x*(1'000'000llu-1);
	}

	std::unordered_map<std::string, int2> pairs;
	for (int64_t i = 0; i < pixels.size(); i++) {
		for (int64_t j = 0; j < pixels.size(); j++) {
			if (i == j) continue;

			int64_t a = i;
			int64_t b = j;
			if (a < b)
				std::swap(a, b);

			std::string hash = std::to_string(a) + ":" + std::to_string(b);
			if (pairs.find(hash) == pairs.end()) {
				pairs[hash] = { a, b };
			}
		}
	}

	int64_t result = 0;
	for (const auto& [hash, pair] : pairs) {
		Pixel* a = &pixels[pair.x];
		Pixel* b = &pixels[pair.y];

		int64_t d = abs(a->x-b->x) + abs(a->y-b->y);
		result += d;
	}

	printf("Result: %zu\n", result);

	return 0;
}

char get_value(int x, int y) {
	for (const auto& p : pixels)
		if (p.x == x && p.y == y)
			return '#';
	return '.';
}