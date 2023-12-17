#include <iostream>
#include <string>
#include <inttypes.h>
#include <vector>
#include <algorithm>

static uint64_t Hash(const char* str) {
	uint64_t result = 0;
	for (const char* c = str; *c; c++) {
		uint64_t v = (uint64_t)*c;
		result += v;
		result *= 17;
		result %= 256;
	}
	return result;
}

enum class Operation {
	Add,
	Remove,
};

struct Step {
	uint64_t hash;
	Operation op;
	std::string label;
	int lens;
};

struct Entry {
	std::string label;
	int lens;
};

static std::vector<Entry> hash_table[256];

static Step MakeStep(const char* str) {
	Step step;
	std::vector<char> label;

	int iter = 0;
	for (const char* c = str; *c; c++) {
		if (isalpha(*c)) {
			label.push_back(*c);
			++iter;
		}
		else {
			break;
		}
	}

	step.label = std::string(label.begin(), label.end());
	step.hash = Hash(step.label.c_str());
	if (str[iter] == '-') {
		step.op = Operation::Remove;
	}
	else {
		step.op = Operation::Add;
		step.lens = str[iter + 1] - '0';
	}

	return step;
}

int main() {

	Hash("HASH");
	Hash("rn");

	std::vector<Step> steps;

	uint64_t result1 = 0;
	for (std::string line; std::getline(std::cin, line);) {
		char* str = _strdup(line.c_str());

		char* tok = strtok(str, ",");

		while (tok != NULL) {
			Step step = MakeStep(tok);
			steps.push_back(step);
			tok = strtok(NULL, ",");
		}

		free(str);
	}

	for (const auto& step : steps) {
		auto& list = hash_table[step.hash];
		auto it = std::find_if(list.begin(), list.end(), [&](const Entry& entry) { return entry.label == step.label; });
		bool exists = it != list.end();
		if (step.op == Operation::Remove) {
			if (exists) {
				list.erase(it);
			}
		}
		else {
			if (!exists) {
				list.push_back({ step.label, step.lens });
			}
			else {
				it->label = step.label;
				it->lens = step.lens;
			}
		}

#if 0
		int i = 0;
		for (const auto& box : hash_table) {
			if (box.size()) {
				std::cout << "Box " << i << ":";

				for (const auto& entry : box) {
					std::cout << " [" << entry.label << " " << entry.lens << "]";
				}
				std::cout << "\n";
			}
			++i;
		}

		std::cout << "\n";
#endif // 0

	}

	size_t result2 = 0;
	for (size_t i = 0; i < 256; i++)
	{
		auto& box = hash_table[i];
		for (size_t j = 0; j < box.size(); j++)
		{
			auto& lens = box[j];
			result2 += (i + 1llu) * (j + 1llu) * lens.lens;
		}
	}

	printf("%zu\n", result2);

	return 0;
}
