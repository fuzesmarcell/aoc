#include <stdio.h>

#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <iostream>
#include <stack>

int64_t possibiility = 0;
static void Arrangements(const std::vector<char>& conds, int ci,
						 const std::vector<int>& groups, int sum,
						 std::vector<char> stack,
						 int& result) {

	int n = (int)std::count_if(stack.begin(), stack.end(), [](char c) { return c == '#'; });
	if (n > sum) {
		return;
	}

	if (stack.size() == conds.size()) {
		if (n != sum) {
			return;
		}

		++possibiility;
	#if 0
		int i = 0;
		int j = 0;
		bool is_valid = true;
		while (i < stack.size()) {
			char c = stack[i];
			if (c == '#') {
				int count = 0;
				while (i < stack.size()) {
					if (stack[i] != '#') {
						break;
					}
					++count;
					++i;
				}

				if (count != groups[j++]) {
					is_valid = false;
					goto computed;
				}
			}
			else {
				++i;
			}
		}

	computed:
		if (is_valid) {
			++result;
		#if 0
			for (auto c : stack) {
				printf("%c", c);
			}
			printf("\n");
		#endif // 0
		}
	#endif // 0


		return;
	}

	char c = conds[ci];
	if (c == '?') {
		stack.push_back('#');
		Arrangements(conds, ci+1, groups, sum, stack, result);
		stack.pop_back();

		stack.push_back('.');
		Arrangements(conds, ci+1, groups, sum, stack, result);
		stack.pop_back();
	}
	else {
		stack.push_back(c);
		Arrangements(conds, ci+1, groups, sum, stack, result);
		stack.pop_back();
	}
}

enum class StateAction {
	Nil,
	DotAndPop,
	Pop,
};

struct State {
	int ci;
	StateAction action;
};

static int64_t ArrangementsIterative(const std::vector<char>& conds, const std::vector<int>& groups, int sum) {

	std::vector<char> combination;

	std::stack<State> stack;
	stack.push({ 0, StateAction::Nil });

	// 77,558,760

	int64_t iters = 0;
	while (stack.size()) {
		auto& state = stack.top();

		if (stack.size() == conds.size()) {
			combination.push_back(conds[state.ci]);

			int n = (int)std::count_if(combination.begin(), combination.end(), [](char c) { return c == '#'; });
			if (n == sum) {
				if (iters % 100'000 == 0) {
					fflush(stdout);
					printf("\r%zu/%zu", iters, 1761039350070);
				}
				++iters;
			}

			combination.pop_back();
			stack.pop();
			continue;
		}

		if (state.action == StateAction::Pop) {
			combination.pop_back();
			stack.pop();
			continue;
		}
		else if (state.action == StateAction::DotAndPop) {
			state.action = StateAction::Pop;
			combination.pop_back();
			combination.push_back('.');
			stack.push({ state.ci+1, StateAction::Nil });
			continue;
		}

		char c = conds[state.ci];
		if (c == '?') {
			if (state.action == StateAction::Nil) {
				state.action = StateAction::DotAndPop;
				combination.push_back('#');
				stack.push({ state.ci+1, StateAction::Nil });
			}
		}
		else {
			state.action = StateAction::Pop;
			combination.push_back(c);
			stack.push({ state.ci+1, StateAction::Nil });
		}
	}

	return iters;
}

static int64_t Factorial(int64_t f) {
	int64_t result = 1;
	for (int64_t i = 2; i <= f; i++) {
		result *= i;
	}
	return result;
}

void GenShifts(const std::vector<int>& a, int ai, int m, std::vector<int>& stack) {

	for (int i = 0; i < m; i++) {

		stack.push_back(i);
		GenShifts(a, ai+1, m, stack);

	}
}

int main() {

#if 0
	std::vector<char> conditions = { '.', '?', '?', '.','.', '?', '?', '.','.','.', '?', '#','#','.', };
	std::vector<int> groups = { 1, 1, 3 };
#endif // 0

	int m = 3;
	std::vector<int> a = { 0, 0, 0 };

	int64_t result1 = 0;
	for (std::string line; std::getline(std::cin, line);) {

		char* str = strdup(line.c_str());

		char* tok = strtok(str, " ");
		std::string s = tok;
		tok = strtok(NULL, " ");
		std::string n = tok;

		free(str);

		std::vector<char> initial_conditions;
		for (auto c : s) {
			initial_conditions.push_back(c);
		}

		std::vector<int> initial_groups;

		str = strdup(n.c_str());

		tok = strtok(str, ",");
		while (tok != NULL) {
			initial_groups.push_back(atoi(tok));
			tok = strtok(NULL, ",");
		}
		free(str);

		std::vector<char> conditions;
		std::vector<int> groups;

		for (int i = 0; i < 5; i++) {
			for (auto c : initial_conditions) {
				conditions.push_back(c);
			}
			conditions.push_back('?');
			for (auto n : initial_groups) {
				groups.push_back(n);
			}
		}

		conditions.pop_back();

		for (auto c : conditions) {
			printf("%c", c);
		}
		printf(" ");
		for (auto n : groups) {
			printf("%d,", n);
		}

		printf("\n");

		possibiility = 0;
		std::vector<char> stack;
		int result = 0;

		int nh = (int)std::count_if(conditions.begin(), conditions.end(), [](char c) { return c == '#'; });
		int nq = (int)std::count_if(conditions.begin(), conditions.end(), [](char c) { return c == '?'; });
		int sum = std::accumulate(groups.begin(), groups.end(), 0);

		int64_t x = ArrangementsIterative(conditions, groups, sum);
		printf("%zu\n", x);

		// Arrangements(conditions, 0, groups, sum, stack, result);
		// printf("%d\n", result);
		result1 += result;
	}

	printf("Result: %zu\n", result1);

	return 0;
}