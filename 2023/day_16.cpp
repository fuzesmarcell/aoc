#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>

union int2 {
	int e[2];
	struct { int x, y; };

	int2() {}
	int2(int e0, int e1) : e{ e0, e1 } {}

	int& operator[](int i) { return e[i]; }
	const int& operator[](int i) const { return e[i]; }
};

static inline bool operator==(const int2& lhs, const int2& rhs) {
	return lhs[0] == rhs[0] && lhs[1] == rhs[1];
}

static inline int2 operator+(int2 lhs, int2 rhs) {
	return { lhs[0] + rhs[0], lhs[1] + rhs[1] };
}

static inline int2& operator+=(int2& lhs, int2 rhs) {
	lhs = lhs + rhs;
	return lhs;
}

static inline int2 operator-(int2 lhs, int2 rhs) {
	return { lhs[0] - rhs[0], lhs[1] - rhs[1] };
}

static inline int2& operator-=(int2& lhs, int2 rhs) {
	lhs = lhs - rhs;
	return lhs;
}

template <typename T>
struct TileMap
{
	int2 size = int2(0, 0);
	std::vector<T> data;

	TileMap() {}

	void AddRow(const T* ptr, size_t len) {
		++size.y;
		size.x = 0;
		for (size_t i = 0; i < len; i++) {
			data.push_back(ptr[i]);
			++size.x;
		}
	}

	bool InBounds(int x, int y) const {
		if (x < 0 || x >= size.x ||
			y < 0 || y >= size.y)
			return false;
		return true;
	}

	bool InBounds(int2 idx) const {
		return InBounds(idx.x, idx.y);
	}

	bool OutOfBounds(int2 idx) const {
		return !InBounds(idx);
	}

	T& operator()(int x, int y) {
		assert(InBounds(x, y));
		return data[y * size.x + x];
	}

	const T& operator()(int x, int y) const {
		assert(InBounds(x, y));
		return data[y * size.x + x];
	}

	T& operator[](int2 idx) {
		assert(InBounds(idx));
		return data[idx.y * size.x + idx.x];
	}

	const T& operator[](int2 idx) const {
		assert(InBounds(idx));
		return data[idx.y * size.x + idx.x];
	}
};

struct Laser {
	int2 pos;
	int2 vel;
	bool retire = false;
};

struct Tile {
	char c;
	bool dir[4];
};

static void Simulate(TileMap<Tile>& map, Laser& laser, std::vector<Laser>& lasers_to_add) {
	laser.pos += laser.vel;

	if (map.OutOfBounds(laser.pos)) {
		laser.retire = true;
		return;
	}

	Tile& tile = map[laser.pos];

	int dir = -1;
	if (laser.vel == int2(1, 0)) {
		dir = 0;
	}
	else if (laser.vel == int2(-1, 0)) {
		dir = 1;
	}
	else if (laser.vel == int2(0, 1)) {
		dir = 2;
	}
	else if (laser.vel == int2(0, -1)) {
		dir = 3;
	}
	assert(dir != -1);

	if (tile.dir[dir]) {
		laser.retire = true;
		return;
	}
	else {
		tile.dir[dir] = true;
	}

	switch (tile.c)
	{
	case '.':
		// nothing to do
		break;

	case '/':
		if (laser.vel == int2(1, 0)) {
			laser.vel = int2(0, -1);
		}
		else if (laser.vel == int2(-1, 0)) {
			laser.vel = int2(0, 1);
		}
		else if (laser.vel == int2(0, 1)) {
			laser.vel = int2(-1, 0);
		}
		else if (laser.vel == int2(0, -1)) {
			laser.vel = int2(1, 0);
		}
		break;

	case '\\':
		if (laser.vel == int2(1, 0)) {
			laser.vel = int2(0, 1);
		}
		else if (laser.vel == int2(-1, 0)) {
			laser.vel = int2(0, -1);
		}
		else if (laser.vel == int2(0, 1)) {
			laser.vel = int2(1, 0);
		}
		else if (laser.vel == int2(0, -1)) {
			laser.vel = int2(-1, 0);
		}
		break;

	case '-':
		if (laser.vel == int2(0, 1) ||
			laser.vel == int2(0, -1)) {
			lasers_to_add.push_back({ laser.pos, int2(-1, 0) });
			lasers_to_add.push_back({ laser.pos, int2(1, 0) });
			laser.retire = true;
			return;
		}
		break;

	case '|':
		if (laser.vel == int2(1, 0) ||
			laser.vel == int2(-1, 0)) {
			lasers_to_add.push_back({ laser.pos, int2(0, 1) });
			lasers_to_add.push_back({ laser.pos, int2(0, -1) });
			laser.retire = true;
			return;
		}
		break;
	default:
		assert(false);
		break;
	}
}

static int SimulateLaserBeam(TileMap<Tile> grid, int2 p, int2 v) {
	std::vector<Laser> lasers;
	lasers.push_back({ p, v });

	while (lasers.size()) {
#if 0
		system("cls");
		for (int y = 0; y < grid.size.y; y++) {
			for (int x = 0; x < grid.size.x; x++) {

				bool is_laser = std::any_of(lasers.begin(), lasers.end(), [&](auto& laser) {
					return laser.pos == int2(x, y);
					});

				if (is_laser) {
					std::cout << "*";
				}
				else {
					std::cout << grid(x, y).c;
				}
			}
			std::cout << "\n";
		}
#endif // 0

		std::vector<Laser> to_add;
		for (int i = 0; i < lasers.size(); i++) {
			Simulate(grid, lasers[i], to_add);
		}

		auto it = std::remove_if(lasers.begin(), lasers.end(), [](auto& laser) { return laser.retire; });
		lasers.erase(it, lasers.end());

		for (auto& laser : to_add)
			lasers.push_back(laser);
	}

	int result = 0;
	for (int y = 0; y < grid.size.y; y++) {
		for (int x = 0; x < grid.size.x; x++) {
			auto& tile = grid(x, y);
			bool any = std::any_of(&tile.dir[0], &tile.dir[4], [](bool b) {return b; });
			if (any) {
				++result;
			}
		}
	}

	return result;
}

int main() {
	TileMap<Tile> grid;
	for (std::string line; std::getline(std::cin, line);) {
		std::vector<Tile> row;
		for (auto c : line) row.push_back({ c, {} });
		grid.AddRow(row.data(), row.size());
	}

	int result = INT_MIN;

	for (int y = 0; y < grid.size.y; y++)
	{
		int e0 = SimulateLaserBeam(grid, int2(-1, y), int2(1, 0));
		int e1 = SimulateLaserBeam(grid, int2(grid.size.x, y), int2(-1, 0));
		if (e0 > result)
			result = e0;
		if (e1 > result)
			result = e1;
	}

	for (int x = 0; x < grid.size.x; x++)
	{
		int e0 = SimulateLaserBeam(grid, int2(x, -1), int2(0, 1));
		int e1 = SimulateLaserBeam(grid, int2(x, grid.size.y - 1), int2(0, -1));
		if (e0 > result)
			result = e0;
		if (e1 > result)
			result = e1;
	}

	return 0;
}
