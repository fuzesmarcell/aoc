#include <iostream>
#include <assert.h>

#include <aoc.h>

uint64_t signal_strength(int64_t cycle, int64_t x) {
	static int64_t c = 20;
	if (cycle != c) {
		return 0;
	}
	c += 40;

	return cycle * x;
}

#define CRT_WIDTH (40)
#define CRT_HEIGHT (6)

void draw(uint32_t* pixels, int64_t cycle, int64_t x) {
	--cycle;
	int64_t px = (cycle % 40);
	int64_t py = cycle / 40;
	if ((px == x) || (px == (x - 1)) || (px == (x + 1))) {
		pixels[py * CRT_WIDTH + px] = 0xffffffff;
	}
}

int main(int argc, char const *argv[])
{
	std::string content = read_entire_file("input.txt");

	uint32_t pixels[CRT_HEIGHT * CRT_WIDTH] = {};

	uint64_t result = 0;
	int64_t num_cycle = 0;
	int64_t x = 1;
	for (const auto line : split(content, "\n")) {
		char buffer[256];
		int res = sscanf(std::string(line).c_str(), "%s", buffer);
		assert(res == 1);

		std::string instruction = buffer;
		if (instruction == "addx") {
			int param;
			int res = sscanf(std::string(line).c_str(), "%s %d", buffer, &param);
			assert(res == 2);

			++num_cycle;
			result += signal_strength(num_cycle, x);

			// draw
			draw(pixels, num_cycle, x);

			++num_cycle;
			result += signal_strength(num_cycle, x);

			// draw
			draw(pixels, num_cycle, x);

			x += param;
		}
		else { // noop
			assert(instruction == "noop");
			++num_cycle;
			result += signal_strength(num_cycle, x);

			// draw
			draw(pixels, num_cycle, x);
		}
	}

	std::cout << "Part 01: " << result << "\n";

	for (int y = 0; y < CRT_HEIGHT; y++) {
		for (int x = 0; x < CRT_WIDTH; x++) {
			uint32_t v = pixels[y * CRT_WIDTH + x];
			if (v) {
				printf("#");
			}
			else {
				printf(".");
			}
		}
		printf("\n");
	}

	return 0;
}