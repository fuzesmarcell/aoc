#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include <string_view>

#include <aoc.h>

int main() {
	std::string content = read_entire_file("input.txt");
	std::array<int, 3> max = { 0, 0, 0 };
	for (const auto groups : split(content, "\n\n")) {
		int sum = 0;
		for (const auto str : split(groups, "\n")) {
			std::cout << str << "\n";
			sum += to_int(str);
		}
		std::cout << "---\n";

		int min = std::numeric_limits<int>::max();
		int min_idx = 0;
		for (int i = 0; i < max.size(); i++) {
			if (max[i] < min) {
				min = max[i];
				min_idx = i;
			}
		}
		if (sum > min) {
			max[min_idx] = sum;
		}
	}

	int result_01 = *std::max_element(max.begin(), max.end());
	std::cout << "Part 01: " << result_01 << "\n";

	int result_02 = std::accumulate(max.begin(), max.end(), 0);
	std::cout << "Part 02: " << result_02 << "\n";
}