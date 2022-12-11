#include <iostream>
#include <regex>
#include <assert.h>

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

enum {
	MUL,
	ADD,
};

struct Monkey {
	uint64_t inspected;
	std::vector<uint64_t> items;
	int expr_type;
	MonkeyFunc* worry_func;
	uint64_t param;
	uint64_t test;
	uint64_t true_idx;
	uint64_t false_idx;
};

struct CongruentStatement {
	uint64_t b;
	uint64_t mod;
};

// https://math.stackexchange.com/questions/2442407/find-number-in-a-certain-range-satisfying-three-modulus-requirements
// https://en.wikipedia.org/wiki/Chinese_remainder_theorem
uint64_t chinese_remainder_theorem(const std::vector<CongruentStatement>& statements) {
	uint64_t result = statements.front().b;
	uint64_t adder = statements.front().mod;
	for (size_t i = 1; i < statements.size(); i++) {
		CongruentStatement s = statements[i];

		while ((result % s.mod) != s.b) {
			result += adder;
		}

		adder *= s.mod;
	}

	return result;
}

int main(int argc, char const *argv[])
{
	{
		std::vector<CongruentStatement> statements;
		statements.push_back({ 20, 23 });
		statements.push_back({  8, 19 });
		statements.push_back({  6, 13 });
		statements.push_back({ 14, 17 });

		assert(chinese_remainder_theorem(statements) == 74678);
	}

	{
		std::vector<CongruentStatement> statements;
		statements.push_back({ 0, 3 });
		statements.push_back({ 3, 4 });
		statements.push_back({ 4, 5 });

		assert(chinese_remainder_theorem(statements) == 39);
	}

	{
		std::vector<CongruentStatement> statements;
		statements.push_back({ 1,  2 });
		statements.push_back({ 2,  5 });
		statements.push_back({ 4, 11 });

		assert(chinese_remainder_theorem(statements) == 37);
	}
	
	
	using std::string_view;
	using StrArr = std::vector<std::string_view>;

	//
	// parsing
	//

	std::string content = read_entire_file("input.txt");
	std::vector<string_view> lines = split(content, "\n");

	std::vector<Monkey> monkeys;
	std::vector<uint64_t> divisors;

	for (size_t i = 0; i < lines.size(); ) {
		StrArr starting_items = split(lines[i + 1], " ");
		StrArr operation = split(lines[i + 2], " ");
		StrArr test = split(lines[i + 3], " ");
		StrArr true_str = split(lines[i + 4], " ");
		StrArr false_str = split(lines[i + 5], " ");

		string_view op = operation[operation.size() - 2];
		string_view param = operation[operation.size() - 1];

		MonkeyFunc* mf = nullptr;

		int e = MUL;
		int p = 0;
		if (param != "old") {
			p = to_int(param);

			if (op == "+") {
				e = ADD;
				mf = old_add_param;
			}
			else {
				e = MUL;
				mf = old_mul_param;
			}
		}
		else {
			e = MUL;
			mf = old_mul_old;
		}

		Monkey m = {
			.expr_type = e,
			.worry_func = mf,
			.param = (uint64_t)p,
			.test = (uint64_t)to_int(test[test.size() - 1]),
			.true_idx = (uint64_t)to_int(true_str[true_str.size() - 1]),
			.false_idx = (uint64_t)to_int(false_str[false_str.size() - 1]),
		};

		divisors.push_back(m.test);

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
	for (size_t i = 0; i < 10000; i++) {
		for (auto& monkey : monkeys) {
			for (auto item : monkey.items) {
				++monkey.inspected;
				uint64_t worry_level = monkey.worry_func(item, monkey.param);
				//worry_level /= 3;

				std::vector<CongruentStatement> statements;
				for (auto divisor : divisors) {
					statements.push_back({
						.b = worry_level % divisor,
						.mod = divisor
						});
				}

				worry_level = chinese_remainder_theorem(statements);

			#if 0
				std::vector<uint64_t> remainders;
				for (auto divisor : divisors) {
					remainders.push_back(worry_level % divisor);
				}

				for (size_t j = 0; j < worry_level; j++) {
					bool match = true;
					for (size_t k = 0; k < divisors.size(); k++) {
						if (j % divisors[k] != remainders[k]) {
							match = false;
							break;
						}
					}

					if (match) {
						worry_level = j;
						break;
					}
				}
			#endif // 0

				//assert(new_worry_level == worry_level);

				uint64_t remainder = worry_level % monkey.test;
				if (remainder == 0) {
					//std::cout << "Throws true " << worry_level << " % " << monkey.test << " == " << remainder << "\n";
					monkeys[monkey.true_idx].items.push_back(worry_level);
				}
				else {
					//std::cout << "Throws false " << worry_level << " % " << monkey.test << " == " << remainder << "\n";
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

	#if 1
		if ((i + 1) % 1000 == 0) {
			std::cout << "== After round " << i + 1 << " ==\n";
			int j = 0;
			for (const auto& monkey : monkeys) {
				std::cout << "Monkey " << j++ << " inspected items " << monkey.inspected << " times\n";
			}
			std::cout << "\n...\n";
		}
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