#include <assert.h>
#include <iostream>
#include <vector>
#include <stack>

#include <aoc.h>

int main() {
	std::string content = read_entire_file("input.txt");
	std::vector<std::string_view> puzzle_str = split(content, "\n\n");
	assert(puzzle_str.size() == 2);

	std::string_view stack_str = puzzle_str[0];
	std::string_view moves = puzzle_str[1];

	std::vector<std::string_view> stack_vec = split(stack_str, "\n");

	std::vector<size_t> package_char_idx;
	for (size_t i = 0; i < stack_vec[stack_vec.size() - 1].size(); i++) {
		char c = stack_vec[stack_vec.size() - 1][i];
		if (std::isalnum(c)) {
			package_char_idx.push_back(i);
		}
	}

	assert(package_char_idx.size());
	std::vector<std::stack<char>> stacks;
	stacks.resize(package_char_idx.size());

	for (int i = (int)stack_vec.size() - 2; i >= 0; i--) {
		int stack_idx = 0;
		std::string_view packages = stack_vec[i];
		for (size_t j = 0; j < package_char_idx.size(); j++) {
			size_t idx = package_char_idx[j];
			char c = packages[idx];
			if (std::isalpha(c)) {
				stacks[j].push(c);
			}
		}
	}

	for (const auto m : split(moves, "\n")) {
		int amount, src, dest;
		int parse_result = sscanf(std::string(m).c_str(), "move %d from %d to %d", &amount, &src, &dest);
		assert(parse_result == 3);
		--src;
		--dest;

		std::vector<char> crates;
		crates.resize(amount);

		for (int i = (int)crates.size() - 1; i >= 0; i--) {
			crates[i] = stacks[src].top();
			stacks[src].pop();
		}

		for (auto c : crates) {
			stacks[dest].push(c);
		}
	}

	for (const auto& stack : stacks) {
		std::cout << stack.top();
	}

	std::cout << "\n";
}