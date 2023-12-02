#include <limits.h>
#include <iostream>
#include <vector>
#include <tuple>

static inline std::tuple<int, int> MaxBank(const std::vector<int>& banks) {
	int max = INT_MIN;
	int result = -1;
	for (size_t i = 0; i < banks.size(); i++) {
		if (banks[i] > max) {
			max = banks[i];
			result = (int)i;
		}
	}

	return { result, max };
}

int main() {
	std::vector<int> input;
	int n;
	while (scanf("%d", &n) != EOF)
		input.push_back(n);

	std::vector<std::vector<int>> history;

	std::vector<int> banks = input;
	size_t iter = 0;
	for (;;) {
		++iter;
		auto [bank, blocks] = MaxBank(banks);
		banks[bank] = 0;

		while (blocks--) {
			int i = ++bank % banks.size();
			++banks[i];
		}

		for (const auto& prev_banks : history) {
			if (prev_banks == banks)
				goto end;
		}

		history.push_back(banks);
	}

end:

	return 0;
}