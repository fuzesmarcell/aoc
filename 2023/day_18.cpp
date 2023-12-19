#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>
#include <vector>
#include <stack>
#include <set>
#include <utility>
#include <numeric>
#include <assert.h>

#include <omp.h>

#include "aoc.hpp"

struct Instruction {
	char d;
	int n;
};

// end-point exclusive
struct Range2i {
	int2 min, max;

	Range2i() {}
	Range2i(int2 begin, int2 end) : min{ begin }, max{ end } {}

	bool inside(int2 p) const {
		if (p.x >= min.x && p.x < max.x &&
			p.y >= min.y && p.y < max.y)
			return true;
		return false;
	}
};

static bool OnAnyLine(const std::vector<Range2i>& ranges, int2 p) {
	for (const auto& range : ranges)
		if (range.inside(p))
			return true;
	return false;
}

static void FloodFill(const std::vector<Range2i>& ranges, int2 p, int2 min, int2 max, size_t& counter) {
	std::set<std::pair<int, int>> visited;

	if (OnAnyLine(ranges, p))
		return;

	std::stack<int2> q;
	q.push(p);

	size_t iter = 0;
	while (q.size()) {
		++iter;
		if (iter % 1'000'000 == 0) {
			fflush(stdout);
			fprintf(stdout, "\r%zu", q.size());
		}

		int2 n = q.top();
		q.pop();

		if (n.x < min.x || n.x >= max.x) {
			counter = 0;
			return;
		}

		if (n.y < min.y || n.y >= max.y) {
			counter = 0;
			return;
		}

		if (OnAnyLine(ranges, n))
			continue;

		auto pos = std::make_pair(n.x, n.y);
		auto it = visited.find(pos);

		if (it == visited.end()) {
			visited.insert(pos);
			++counter;

			q.push(n + int2(1, 0));
			q.push(n + int2(-1, 0));
			q.push(n + int2(0, 1));
			q.push(n + int2(0, -1));
		}
	}

	printf("\n");
}

static bool IsPointIntPath(const std::vector<Range2i>& ranges, int2 p) {
	if (OnAnyLine(ranges, p))
		return true;

	size_t counter = 0;
	for (size_t i = 0; i < ranges.size(); i++) {
		auto& range = ranges[i];
		bool is_vertical = range.min.x == (range.max.x - 1);
		if (is_vertical) {
			if (p.x < range.min.x &&
				p.y >= range.min.y &&
				p.y < range.max.y) {
				++counter;
			}
		}
		else {
			int j = (i + 1) % ranges.size();
			int k = i - 1;
			if (k < 0)
				k = ranges.size() - 1;

			auto& a = ranges[j];
			auto& b = ranges[k];

			bool above0 = a.min.y < range.min.y;
			bool above1 = b.min.y < range.min.y;

			bool below0 = (a.max.y - 1) > range.min.y;
			bool below1 = (b.max.y - 1) > range.min.y;

			bool is_edge = (above0 && above1) || (below0 && below1);

			if (!is_edge &&
				p.x < range.min.x &&
				p.y == range.min.y) {
				--counter;
			}
		}
	}
	return counter % 2 != 0;
}

static int NextVerticalEdge(const std::vector<Range2i>& ranges, int2 p) {
	if (OnAnyLine(ranges, p))
		return INT_MAX;

	int closest_x = INT_MAX;
	for (auto& range : ranges) {
		bool is_vertical = range.min.x == (range.max.x - 1);
		if (is_vertical) {
			if (p.y >= range.min.y && p.y < range.max.y) {
				if (range.min.x > p.x && range.min.x < closest_x) {
					closest_x = range.min.x;
				}
			}
		}
	}

	return closest_x;
}

static std::vector<std::pair<int, int>> TestAllVertical(const std::vector<Range2i>& ranges, int2 p) {
	std::vector<std::pair<int, int>> result;
	for (int i = 0; i < (int)ranges.size(); i++) {
		auto& range = ranges[i];
		bool is_vertical = range.min.x == (range.max.x - 1);
		if (is_vertical) {
			if (p.y >= range.min.y && p.y < range.max.y) {
				if (range.min.x > p.x) {
					result.push_back(std::make_pair(range.min.x, i));
				}
			}
		}
	}

	std::sort(result.begin(), result.end(), [](auto& a, auto& b) {
		return a.first < b.first;
		});

	return result;
}

static std::vector<std::pair<int, int>> TestAllHorizontal(const std::vector<Range2i>& ranges, int2 p) {
	std::vector<std::pair<int, int>> result;
	for (int i = 0; i < (int)ranges.size(); i++) {
		auto& range = ranges[i];
		bool is_vertical = range.min.x == (range.max.x - 1);
		if (!is_vertical) {
			if (p.y == range.min.y) {
				if (p.x < range.min.x) {
					result.push_back(std::make_pair(range.min.x, i));
				}
			}
		}
	}

	std::sort(result.begin(), result.end(), [](auto& a, auto& b) {
		return a.first < b.first;
		});

	return result;
}

int main() {


#if 0
	std::vector<Range2i> ranges;
	ranges.push_back(Range2i(int2(1, 0), int2(8, 0) + int2(1, 1)));
	ranges.push_back(Range2i(int2(8, 0), int2(8, 6) + int2(1, 1)));
	ranges.push_back(Range2i(int2(4, 6), int2(8, 6) + int2(1, 1)));
	ranges.push_back(Range2i(int2(4, 4), int2(4, 6) + int2(1, 1)));
	ranges.push_back(Range2i(int2(1, 4), int2(4, 4) + int2(1, 1)));
	ranges.push_back(Range2i(int2(1, 0), int2(1, 4) + int2(1, 1)));

#if 0
	ranges.push_back(Range2i(int2(0, 0), int2(10, 1)));
	ranges.push_back(Range2i(int2(9, 0), int2(10, 10)));
	ranges.push_back(Range2i(int2(0, 9), int2(10, 10)));
	ranges.push_back(Range2i(int2(0, 0), int2(1, 10)));
#endif // 0


	int2 min(INT_MAX, INT_MAX);
	int2 max(INT_MIN, INT_MIN);
	for (const auto& range : ranges) {
		min = Min(range.min, min);
		min = Min(range.max, min);
		max = Max(range.min, max);
		max = Max(range.max, max);
	}

	min -= int2(1, 1);
	max += int2(1, 1);

	for (int y = min.y; y < max.y; y++) {
		for (int x = min.x; x < max.x; x++) {
			if (IsPointIntPath(ranges, int2(x, y))) {
				std::cout << "#";
			}
			else {
				std::cout << ".";
			}
		}
		std::cout << "\n";
	}

	size_t count = 0;
	for (int y = min.y; y < max.y; y++) {
		int2 p(-1, y);
		auto v = TestAllVertical(ranges, p);
		auto h = TestAllHorizontal(ranges, p);

		if (v.size() && h.size()) {
			size_t i = v.size() - 1;
			for (int x = v[0].first; x <= v[i].first; x++) {
				if (IsPointIntPath(ranges, int2(x, y))) {
					++count;
				}
			}
		}
		else if (v.size()) {
			assert(v.size() % 2 == 0);
			for (size_t i = 0; i < v.size(); i += 2) {
				count += ((size_t)v[i + 1].first - v[i].first) + 1;
			}

		}
	}

	int stophere = 5;
#endif // 0


	std::vector<Instruction> instructions;
	std::regex re(R"(\(#(.+)\))");
	for (std::string line; std::getline(std::cin, line);) {
		std::smatch m;
		std::regex_search(line, m, re);
		assert(m.size() == 2);

		std::string hex_str = m[1].str();

		std::vector<char> n;
		for (size_t i = 0; i < hex_str.length() - 1; i++)
			n.push_back(hex_str[i]);

		char d = hex_str[hex_str.length() - 1];
		if (d == '0')
			d = 'R';
		else if (d == '1')
			d = 'D';
		else if (d == '2')
			d = 'L';
		else if (d == '3')
			d = 'U';
		else
			assert(false);

		std::string nstr(n.begin(), n.end());

		Instruction in;
		in.d = d;
		in.n = strtoul(nstr.c_str(), nullptr, 16);
		instructions.push_back(in);
	}

	std::vector<Range2i> lines;

	int2 pos = int2(0, 0);
	size_t counter0 = 0;

	for (const auto& ins : instructions) {
		counter0 += ins.n;
		switch (ins.d)
		{
		case 'R':
		case 'L': {
			int2 begin = pos;
			int2 o = ins.d == 'R' ? int2(1, 0) : int2(-1, 0);
			for (int i = 0; i < ins.n; i++) {
				pos += o;
			}
			int2 end = pos;

			if (begin.x > end.x)
				std::swap(begin, end);

			lines.push_back({ begin, end + int2(1, 1) });

		} break;

		case 'U':
		case 'D': {
			int2 begin = pos;
			int2 o = ins.d == 'U' ? int2(0, -1) : int2(0, 1);
			for (int i = 0; i < ins.n; i++) {
				pos += o;
			}
			int2 end = pos;
			if (begin.y > end.y)
				std::swap(begin, end);

			lines.push_back({ begin, end + int2(1, 1) });
		} break;

		default:
			assert(false);
			break;
		}
	}

	int2 min(INT_MAX, INT_MAX);
	int2 max(INT_MIN, INT_MIN);
	for (const auto& range : lines) {
		min = Min(range.min, min);
		min = Min(range.max, min);
		max = Max(range.min, max);
		max = Max(range.max, max);
	}

	max += int2(1, 1);

	int nthreads = omp_get_num_procs();
	std::vector<size_t> results(nthreads);

	int iter = 0;
#pragma omp parallel for
	for (int y = min.y; y < max.y; y++) {
		int tid = omp_get_thread_num();

		int2 p(min.x - 1, y);
		auto v = TestAllVertical(lines, p);
		auto h = TestAllHorizontal(lines, p);

		if (v.size() && h.size()) {
			int first_x = Min(v[0].first, v[0].first);
			int last_x = Max(v[v.size() - 1].first, v[h.size() - 1].first);

			int x = first_x;
			while (x <= last_x) {
				auto it = std::find_if(h.begin(), h.end(), [&](auto& pair) {
					return pair.first == x;
					});

				if (it != h.end()) {
					results[tid] += (size_t)lines[it->second].max.x - x;
					x = lines[it->second].max.x;
				}
				else {
					if (IsPointIntPath(lines, int2(x, y))) {
						++results[tid];
					}
					++x;
				}
			}
		}
		else if (v.size()) {
			assert(v.size() % 2 == 0);
			for (size_t i = 0; i < v.size(); i += 2) {
				results[tid] += ((size_t)v[i + 1].first - v[i].first) + 1;
			}

		}
	}

	size_t result = std::accumulate(results.begin(), results.end(), 0llu);

	printf("\nResult: %zu\n", result);

	return 0;
}
