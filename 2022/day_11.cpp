#include <iostream>
#include <regex>

#include <aoc.h>

typedef uint64_t (MonkeyFunc)(uint64_t old, uint64_t param);

uint64_t old_mul_param(uint64_t old, uint64_t param) {
	return old * param;
}

uint64_t old_mul_old(uint64_t old, uint64_t /*param*/) {
	return old * old;
}

uint64_t old_add_param(uint64_t old, uint64_t param) {
	return old + param;
}

struct Monkey {
	uint64_t inspected;
	std::vector<uint64_t> items;
	MonkeyFunc* worry_func;
	uint64_t param;
	uint64_t test;
	uint64_t true_idx;
	uint64_t false_idx;
};

int main(int argc, char const *argv[])
{
	using std::string_view;
	using StrArr = std::vector<std::string_view>;

	//
	// parsing
	//

	std::string content = read_entire_file("input.txt");
	std::vector<string_view> lines = split(content, "\n");

	std::vector<Monkey> monkeys;

	for (size_t i = 0; i < lines.size(); ) {
		StrArr starting_items = split(lines[i + 1], " ");
		StrArr operation = split(lines[i + 2], " ");
		StrArr test = split(lines[i + 3], " ");
		StrArr true_str = split(lines[i + 4], " ");
		StrArr false_str = split(lines[i + 5], " ");

		string_view op = operation[operation.size() - 2];
		string_view param = operation[operation.size() - 1];

		MonkeyFunc* mf = nullptr;

		int p = 0;
		if (param != "old") {
			p = to_int(param);

			if (op == "+") {
				mf = old_add_param;
			}
			else {
				mf = old_mul_param;
			}
		}
		else {
			mf = old_mul_old;
		}

		Monkey m = {
			.worry_func = mf,
			.param = (uint64_t)p,
			.test = (uint64_t)to_int(test[test.size() - 1]),
			.true_idx = (uint64_t)to_int(true_str[true_str.size() - 1]),
			.false_idx = (uint64_t)to_int(false_str[false_str.size() - 1]),
		};

		std::regex r("\\d+");
		std::smatch match;
		std::string str = std::string(lines[i+1]);
		while (std::regex_search(str, match, r)) {
			m.items.push_back(std::stoi(match[0]));
			str = match.suffix().str(); // Proceed to the next match
		}

		monkeys.push_back(m);

		i += 7;
	}
	
	//
	// logic
	//
	for (size_t i = 0; i < 20; i++) {
		for (auto& monkey : monkeys) {
			for (auto item : monkey.items) {
				++monkey.inspected;
				uint64_t worry_level = monkey.worry_func(item, monkey.param);
				worry_level /= 3;
				if (worry_level % monkey.test == 0) {
					monkeys[monkey.true_idx].items.push_back(worry_level);
				}
				else {
					monkeys[monkey.false_idx].items.push_back(worry_level);
				}
			}
			monkey.items.resize(0);
		}

	#if 0
		int j = 0;
		for (const auto& monkey : monkeys) {
			std::cout << "Monkey " << j++ << ": ";
			for (auto item : monkey.items) {
				std::cout << item << ",";
			}
			std::cout << "\n";
		}
		std::cout << "\n...\n";
	#endif // 0

	}

	uint64_t max[2] = {};
	for (const auto& monkey : monkeys) {
		uint64_t* min = max[0] > max[1] ? &max[1] : &max[0];
		if (monkey.inspected > *min) {
			*min = monkey.inspected;
		}
	}

	uint64_t result = max[0] * max[1];
	std::cout << "Part 01: " << result << "\n";

	return 0;
}