#include <iostream>
#include <assert.h>
#include <algorithm>

#include <aoc.h>

#define countof(arr) (sizeof(arr)/sizeof((arr)[0]))

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

	case 'R':
	{
		pos.x += 1;
	} break;

	case 'L':
	{
		pos.x -= 1;
	} break;

	case 'U':
	{
		pos.y += 1;
	} break;

	case 'D':
	{
		pos.y -= 1;
	} break;

	default:
	{
		assert(!"invalid_default_case");
	} break;
	}

	return pos;
}

int main(int argc, char const* argv[]) {
	std::string content = read_entire_file("input.txt");

	int2 rope[10] = {};
	int2* head = &rope[0];
	int2* tail = &rope[countof(rope)-1];

	std::vector<int2> visited;
	visited.push_back(*tail);

	for (auto const line : split(content, "\n")) {
		char d; int n;
		int result = sscanf(std::string(line).c_str(), "%c %d", &d, &n);
		assert(result == 2);

		for (size_t i = 0; i < n; i++) {
			*head = move(d, *head);

			for (size_t j = 1; j < countof(rope); j++) {
				const int2* h = &rope[j - 1];
				int2* pos = &rope[j];

				bool touching = labs(h->x - pos->x) <= 1 &&
					labs(h->y - pos->y) <= 1;
				if (touching) { break; }
				assert(!(h->x == pos->x && h->y == pos->y)); // not eq

				int2 offset = *h - *pos;
				offset.x = offset.x == 0 ? 0 : sign(offset.x);
				offset.y = offset.y == 0 ? 0 : sign(offset.y);
				*pos += offset;
			}

			// add to visited if it does not exist
			bool inside = std::any_of(visited.begin(), visited.end(), [tail](int2 p) { return tail->x == p.x && tail->y == p.y; });
			if (!inside) {
				visited.push_back(*tail);
			}
		}
	}

	std::cout << "Part 01: " << visited.size() << "\n";

	return 0;
}