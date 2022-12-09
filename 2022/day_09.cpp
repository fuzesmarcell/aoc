#include <iostream>
#include <assert.h>
#include <algorithm>

#include <aoc.h>

struct int2 {
	int x, y;
};

int2 make_int2(int x, int y) {
	return int2{ x, y };
}

int2 operator-(int2 a, int2 b) {
	int2 result = make_int2(
		a.x - b.x,
		a.y - b.y);
	return result;
}

int2 operator+=(int2& a, int2 b) {
	a = make_int2(
		a.x + b.x,
		a.y + b.y);

	return a;
}

int sign(int v) {
	return v > 0 ? 1 : -1;
}

int2 move(char move, int2 pos) {
	switch (move) { // move head

	case 'R': {
		pos.x += 1;
	} break;

	case 'L': {
		pos.x -= 1;
	} break;

	case 'U': {
		pos.y += 1;
	} break;

	case 'D': {
		pos.y -= 1;
	} break;

	default: {
		assert(!"invalid_default_case");
	} break;
	}

	return pos;
}

int main(int argc, char const *argv[])
{
	std::string content = read_entire_file("input.txt");

	int2 head = {};
	int2 tail = {};

	std::vector<int2> visited;
	visited.push_back(tail);

	for (auto const line : split(content, "\n")) {
		char d; int n;
		int result = sscanf(std::string(line).c_str(), "%c %d", &d, &n);
		assert(result == 2);

		for (size_t i = 0; i < n; i++) {
			head = move(d, head);

			bool touching = labs(head.x - tail.x) <= 1 &&
				            labs(head.y - tail.y) <= 1;
			if (touching) { continue; }

			assert(!(head.x == tail.x && head.y == tail.y)); // not eq
			bool is_same_row_col = head.x == tail.x ||
				                   head.y == tail.y;
			if (is_same_row_col) {
				tail = move(d, tail);
			}
			else { // diagonal move
				int2 offset = head - tail;
				assert(offset.x != 0 && offset.y != 0);

				offset.x = sign(offset.x);
				offset.y = sign(offset.y);
				tail += offset;
			}

			// add to visited if it does not exist
			bool inside = std::any_of(visited.begin(), visited.end(), [tail](int2 p) { return tail.x == p.x && tail.y == p.y;});
			if (!inside) {
				visited.push_back(tail);
			}
		}
	}

	std::cout << "Part 01: " << visited.size() << "\n";

	return 0;
}