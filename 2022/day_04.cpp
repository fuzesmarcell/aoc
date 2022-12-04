#include <iostream>
#include <assert.h>

#include <aoc.h>

int main() {

	std::string content = read_entire_file("input.txt");

	uint64_t result_01 = 0;
	uint64_t result_02 = 0;
	for (auto const& line : split(content, "\n")) {
		int min_a, max_a;
		int min_b, max_b;
		int result = sscanf(std::string(line).c_str(), "%d-%d,%d-%d", &min_a, &max_a, &min_b, &max_b);
		assert(result == 4);

		int overlap = 0;
		if (min_a >= min_b && max_a <= max_b) {
			overlap = 1;
		}
		else if (min_b >= min_a && max_b <= max_a) {
			overlap = 1;
		}
		result_01 += overlap;

		if (!overlap) {
			if ((min_a >= min_b && min_a <= max_b) ||
				(max_a >= min_b && max_a <= max_b)) {
				overlap = 1;
			}
		}

		result_02 += overlap;
	}

	std::cout << "Part 01: " << result_01 << "\n";
	std::cout << "Part 02: " << result_02 << "\n";

	return 0;
}