#include <stdio.h>
#include <array>
#include <string>
#include <vector>
#include <iostream>

// is `b` a anagram of `a`
static bool IsAnagram(const std::string& a, const std::string& b) {
	if (a.length() != b.length())
		return false;

	struct CharCoutner {
		std::array<int, 'z'-'a'+1> counts = {};

		// `c` must be a lower-case asci char
		void count(char c) {
			int i = 'z' - c;
			++counts[i];
		}

		void count(const std::string& str) {
			for (auto c : str)
				count(c);
		}

		bool operator==(CharCoutner& other) {
			for (size_t i = 0; i < counts.size(); i++) {
				if (counts[i] != other.counts[i])
					return false;
			}

			return true;
		}
	};

	CharCoutner acc;
	acc.count(a);

	CharCoutner bcc;
	bcc.count(b);

	return acc == bcc;
}

int main() {
	size_t result = 0;

	for (std::string str; std::getline(std::cin, str);) {
		char* phrase = _strdup(str.c_str());

		std::vector<std::string> words;

		char* tok = strtok(phrase, " ");
		while (tok != NULL) {
			words.push_back(tok);
			tok = strtok(NULL, " ");
		}
		free(phrase);

		bool is_valid = true;
		for (size_t i = 0; i < words.size(); i++) {
			for (size_t j = 0; j < words.size(); j++) {
				if (i == j)
					continue;
				if (IsAnagram(words[i], words[j]) || words[i] == words[j])
					goto next;
			}
		}
		++result;
	next:;
	}

	printf("%zu\n", result);

	return 0;
}