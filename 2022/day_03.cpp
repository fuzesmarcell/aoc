#include <iostream>
#include <assert.h>
#include <algorithm>
#include <array>

#include <aoc.h>

static int get_priority(char c) {
	assert(std::isalnum(c));
	if (std::isupper(c)) {
		return (c - 'A') + 27;
	}
	
	assert(std::islower(c));
	return (c - 'a') + 1;
}

void part_01(void) {
	std::string content = read_entire_file("input.txt");

	uint64_t result_01 = 0;
	for (const auto& line : split(content, "\n")) {
		assert(line.length() % 2 == 0);
		size_t end_idx = line.length();
		size_t mid_idx = end_idx / 2;
		std::string_view compartment1 = line.substr(0, mid_idx);
		std::string_view compartment2 = line.substr(mid_idx, end_idx);

		char found_char = '\0';
		for (auto c : compartment1) {
			bool exists = std::find(compartment2.begin(), compartment2.end(), c) != compartment2.end();
			if (exists) {
				found_char = c;
				break;
			}
		}

		int priority = get_priority(found_char);

		result_01 += priority;
	}

	std::cout << "Part 01: " << result_01 << "\n";
}

void part_02(void) {
	std::string content = read_entire_file("input.txt");
	std::vector<std::string_view> lines = split(content, "\n");
	assert(lines.size() % 3 == 0);
	uint64_t result = 0;
	for (size_t i = 0; i < lines.size(); i += 3) {
		std::array<std::string_view, 3> group = {
			lines[i],
			lines[i + 1],
			lines[i + 2],
		};

		char found_char = 0;
		for (size_t j = 0; j < 3; j++) {
			for (auto c : group[j]) {
				int found_count = 0;
				for (size_t k = 0; k < 3; k++) {
					if (j == k) { continue; }
					if (inside(group[k], c)) {
						++found_count;
					}
				}

				if (found_count == 2) {
					found_char = c;
					goto found;
				}
			}
		}

	found:
		result += get_priority(found_char);
	}

	std::cout << "Part 02: " << result << "\n";
}

int main() {
	part_01();
	part_02();

	return 0;
}