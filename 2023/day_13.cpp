#include <stdio.h>
#include <assert.h>

#include <string>
#include <iostream>
#include <vector>
#include <utility>

struct Pattern {
	int nx, ny;
	std::vector<char> c;

	const char& operator() (int row, int col) const {
		assert(row >= 0 && row < ny);
		assert(col >= 0 && col < nx);
		return c[row*nx + col];
	}

	char& operator() (int row, int col) {
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

static int FindHorizontal(const Pattern& pattern, int row_to_ignore = -1) {
	std::vector<int> hrows;
	for (int i = 0; i < pattern.ny-1; i++) {
		if (i == row_to_ignore)
			continue;

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
				return hrow;
			}
		}
	}

	return -1;
}

static int FindVertical(const Pattern& pattern, int col_to_ignore = -1) {
	std::vector<int> vcols;
	for (int i = 0; i < pattern.nx-1; i++) {
		if (col_to_ignore == i)
			continue;

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
				return vcol;
			}
		}
	}

	return -1;
}

static std::pair<int, int> FindReflection(const Pattern& pattern, int v = -1, int h = -1) {
	return { FindVertical(pattern, v), FindHorizontal(pattern, h) };
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

	int nv = 0;
	int nh = 0;
	for (const auto& pattern : patterns) {

		auto [vi, hi] = FindReflection(pattern);

		Pattern new_pattern = pattern;
		for (int row = 0; row < new_pattern.ny; row++) {
			for (int col = 0; col < new_pattern.nx; col++) {
				char& v = new_pattern(row, col);
				if (v == '#') {
					v = '.';
				}
				else {
					v = '#';
				}

				auto [nnv, nnh] = FindReflection(new_pattern, vi, hi);
				if (nnv >= 0) {
					nv += nnv+1;
					goto done;
				}
				if (nnh >= 0) {
					nh += nnh+1;
					goto done;
				}

				if (v == '#') {
					v = '.';
				}
				else {
					v = '#';
				}
			}
		}
	done:;
	}

	int64_t result = (int64_t)nv + (int64_t)100*nh;

	return 0;
}