#include <iostream>
#include <aoc.h>

struct Grid {
	int w, h;
	int* grid;

	void set(int x, int y, int v);
	int get(int x, int y) const;
};

#define OUT_OF_BOUNDS (-1)

int main(int argc, char const *argv[])
{
	std::string content = read_entire_file("input.txt");

	std::vector<std::string_view> lines = split(content, "\n");

	Grid grid = {
		.w = (int)lines[0].length(),
		.h = (int)lines.size(),
		.grid = nullptr,
	};

	grid.grid = (int*)calloc(1, grid.w * grid.h * sizeof(int));
	if (!grid.grid) {
		std::cerr << "Failed to allocate grid\n";
		return EXIT_FAILURE;
	}

	for (int y = 0; y < grid.h; y++) {
		for (int x = 0; x < grid.h; x++) {
			grid.set(x, y, lines[y][x] - '0');
		}
	}

	size_t part_02 = 0;
	for (int y = 1; y < (grid.h - 1); y++) {
		for (int x = 1; x < (grid.w - 1); x++) {
			int offsets[4][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };

			int current_height = grid.get(x, y);

			size_t distances[4] = {};
			for (size_t i = 0; i < 4; i++) {
				int x_off = offsets[i][0];
				int y_off = offsets[i][1];

				int it_x = x;
				int it_y = y;
				for (;;) {
					it_x += x_off;
					it_y += y_off;

					int height = grid.get(it_x, it_y);
					if (height == OUT_OF_BOUNDS) {
						// it is visible because we reached the boundaries
						// printf("{%d, %d} %d\n", x, y, grid.get(x, y));
						break;
					}

					++distances[i];
					if (height >= current_height) {
						// not visible
						break;
					}
				}
			}

		next_tree:
			size_t scenic_score = distances[0] * distances[1] * distances[2] * distances[3];
			if (scenic_score > part_02) {
				part_02 = scenic_score;
			}
		}
	}

	std::cout << "Part 02: " << part_02 << "\n";

	return EXIT_SUCCESS;
}

void Grid::set(int x, int y, int v) {
	this->grid[y * this->w + x] = v;
}

int Grid::get(int x, int y) const {
	if (x < 0 || x >= this->w ||
		y < 0 || y >= this->h) {
		return OUT_OF_BOUNDS;
	}

	return this->grid[y * this->w + x];
}
