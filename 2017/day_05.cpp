#include <string>
#include <vector>
#include <iostream>
#include <inttypes.h>

int main() {
	uint64_t result = 0;

	std::vector<int> instructions;
	for (std::string str; std::getline(std::cin, str);) {
		instructions.push_back(atoi(str.c_str()));
	}

	int64_t ip = 0;
	while (ip >= 0 && ip < (int64_t)instructions.size()) {
		int64_t offset = instructions[ip];

		if (offset > 2)
			--instructions[ip];
		else
			++instructions[ip];

		ip += offset;
		++result;
	}

	std::cout << "Result: " << result << "\n";

	return 0;
}