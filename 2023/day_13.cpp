#include <stdio.h>
#include <assert.h>

#include <string>
#include <iostream>
#include <vector>

struct Pattern {
	int nx, ny;
	std::vector<char> c;

	const char& operator() (int row, int col) const {
		assert(row >= 0 && row < ny);
		assert(col >= 0 && col < nx);
		return c[row*nx + col];
	}

	bool RowEq(int row0, int row1) const {
		for (int col = 0; col < nx; col++) {
			char a = operator()(row0, col);
			char b = operator()(row1, col);
			if (a != b)
				return false;
		}
		return true;
	}

	bool ColEq(int col0, int col1) const {
		for (int row = 0; row < ny; row++) {
			char a = operator()(row, col0);
			char b = operator()(row, col1);
			if (a != b)
				return false;
		}
		return true;
	}
};

int64_t fucking_h = 0;
int64_t fucking_v = 0;

static int FindReflection(const Pattern& pattern) {

	std::vector<int> h_results;
	std::vector<int> v_results;

	std::vector<int> hrows;
	for (int i = 0; i < pattern.ny-1; i++) {
		if (pattern.RowEq(i, i+1)) {
			hrows.push_back(i);
		}
	}

	if (hrows.size() > 0) {
		for (auto hrow : hrows) {
			auto IsMirrored = [&](int row) {
				int i = row-1;
				int j = row+2;

				while (i >= 0 && j < pattern.ny) {
					if (!pattern.RowEq(i, j)) {
						return false;
					}
					--i;
					++j;
				}

				return true;
			};

			if (IsMirrored(hrow)) {
				fucking_h += (int64_t)hrow+1;
				return 0;
			}
		}
	}

	std::vector<int> vcols;
	for (int i = 0; i < pattern.nx-1; i++) {
		if (pattern.ColEq(i, i+1)) {
			vcols.push_back(i);
		}
	}

	if (vcols.size() > 0) {
		for (auto vcol : vcols) {
			auto IsMirrored = [&](int col) {
				int i = col - 1;
				int j = col + 2;

				while (i >= 0 && j < pattern.nx) {
					if (!pattern.ColEq(i, j)) {
						return false;
					}
					--i;
					++j;
				}

				return true;
			};

			if (IsMirrored(vcol)) {
				fucking_v += (int64_t)vcol+1;
				return 0;
			}
		}
	}

	assert(v_results.size() == 0 || v_results.size() == 1);
	assert(h_results.size() == 0 || h_results.size() == 1);

	printf("V: %zu\n", v_results.size());
	printf("H: %zu\n", h_results.size());
	printf("---\n");

	return 0;
}

int main() {
	std::vector<Pattern> patterns;
	patterns.push_back({});

	int p_idx = 0;
	for (std::string line; std::getline(std::cin, line);) {
		if (line == "") {
			patterns.push_back({});
			++p_idx;
			continue;
		}

		auto& pattern = patterns[p_idx];
		pattern.nx = 0;
		for (auto c : line) {
			pattern.c.push_back(c);
			++pattern.nx;
		}
		++pattern.ny;
	}

	for (const auto& pattern : patterns) {
		FindReflection(pattern);
	}

	int64_t result = fucking_v + 100*fucking_h;

	return 0;
}