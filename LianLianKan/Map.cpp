#include "Map.h"

#define WIDTH 18
#define HEIGHT 10

using namespace std;

Map::Map() {

}

Map::~Map() {

}


bool Map::full_true(vector<bool> flags) {
	for (auto flag : flags)
		if (!flag) return false;
	return true;
}

void Map::recover(int &x1, int &y1, int &x2, int &y2, int tmp_x1, int tmp_y1,
	int tmp_x2, int tmp_y2) {
	swap(x1, tmp_x1);
	swap(y1, tmp_y1);
	swap(x2, tmp_x2);
	swap(y2, tmp_y2);
}

bool Map::isLineLinkable(int x1, int y1, int x2, int y2) {
	if (x1 == x2) {
		if (y1 > y2) std::swap(y1, y2);

		for (y1 += 1; y1 < y2; ++y1)
			if (map[x1][y1]) return false;

	}
	else if (y1 == y2) {
		if (x1 > x2) std::swap(x1, x2);

		for (x1 += 1; x1 < x2; ++x1)
			if (map[x1][y1]) return false;
	}
	else
		return false;

	return true;
}

vector<vector<int>> Map::makeMap() {
	map.clear();
	vector<bool> flags;
	vector<vector<int>> position;
	int frequency[41];
	memset(frequency, 0, sizeof(frequency));
	srand((unsigned)time(NULL));
	int order = 0;
	order = 1 + (rand() % 16);
	string filename = "./Maps/Map" + std::to_string(order) + ".txt";
	// string filename = "./Maps/Map3.txt";
	fstream readMap;
	readMap.open(filename.c_str());
	int row = 0;
	int col = 0;
	if (!readMap.is_open()) {
		throw "MAP NOT FOUND";
	}
	else {
		vector<int> elements;
		while (!readMap.eof()) {
			char i;
			if (col < WIDTH) {
				readMap.get(i);
				elements.push_back((int)i - '0');
				int a = (int)i - '0';
				if (i == '1') {
					position.push_back({ row, col });
					flags.push_back(false);
				}
				col++;
			}
			else {
				col = 0;
				row++;
				map.push_back(elements);
				elements.clear();
				readMap.get(i);
			}
		}
		readMap.close();
	}
	int len = position.size();
	while (1) {
		int element1 = rand() % len;
		while (flags[element1]) element1 = rand() % len;
		int element2 = rand() % len;
		while (element2 == element1 || flags[element2]) element2 = rand() % len;
		flags[element1] = true;
		flags[element2] = true;
		int rand_element = 1 + (rand() % 40);
		while (1) {
			if (frequency[rand_element] < 3) {
				frequency[rand_element]++;
				break;
			}
			else
				rand_element = 1 + (rand() % 40);
		}
		map[position[element1][0]][position[element1][1]] = rand_element;
		map[position[element2][0]][position[element2][1]] = rand_element;
		if (full_true(flags)) break;
	}
	return map;
}

vector<vector<int>> Map::connection(int x1, int y1, int x2, int y2) {
	vector<vector<int>> route;
	bool flag = false;
	int tmp_x1 = x1;
	int tmp_x2 = x2;
	int tmp_y1 = y1;
	int tmp_y2 = y2;
	if (map[x1][y1] != map[x2][y2] || (x1 == x2 && y1 == y2)) return route;
	if (x1 == x2 && isLineLinkable(x1, y1, x2, y2)) {
		if (y1 > y2) swap(y1, y2);
		for (y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
		map[tmp_x1][tmp_y1] = 0;
		map[tmp_x2][tmp_y2] = 0;
		return route;
	}
	recover(x1, y1, x2, y2, tmp_x1, tmp_y1, tmp_x2, tmp_y2);
	if (y1 == y2 && isLineLinkable(x1, y1, x2, y2)) {
		if (x1 > x2) swap(x1, x2);
		for (x1; x1 <= x2; x1++) route.push_back({ x1, y1 });
		map[tmp_x1][tmp_y1] = 0;
		map[tmp_x2][tmp_y2] = 0;
		return route;
	}
	recover(x1, y1, x2, y2, tmp_x1, tmp_y1, tmp_x2, tmp_y2);
	if (y1 > y2) {
		swap(x1, x2);
		swap(y1, y2);
	}
	for (int i = 0; i < HEIGHT; i++)
		if (!map[i][y1] && isLineLinkable(i, y1, x1, y1)) {
			if (!map[i][y2] && isLineLinkable(i, y2, x2, y2)) {
				if (i >= x1) {
					flag = true;
					for (x1; x1 <= i; x1++) route.push_back({ x1, y1 });
					for (y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
				}
				else {
					for (x1; x1 >= i; x1--) route.push_back({ x1, y1 });
					for (y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
				}
				y1--;
				if (flag)
					x1--;
				else
					x1++;
				if (x1 > x2)
					for (x1; x1 >= x2; x1--) route.push_back({ x1, y1 });
				else
					for (x1; x1 <= x2; x1++) route.push_back({ x1, y1 });
				map[tmp_x1][tmp_y1] = 0;
				map[tmp_x2][tmp_y2] = 0;
				return route;
			}
		}
	recover(x1, y1, x2, y2, tmp_x1, tmp_y1, tmp_x2, tmp_y2);
	flag = false;
	if (x1 < x2) {
		swap(x1, x2);
		swap(y1, y2);
	}
	for (int i = 0; i < WIDTH; i++) {
		if (!map[x1][i] && isLineLinkable(x1, i, x1, y1)) {
			if (!map[x2][i] && isLineLinkable(x2, i, x2, y2)) {
				if (i >= y1) {
					flag = true;
					for (y1; y1 <= i; y1++) route.push_back({ x1, y1 });
					for (x1; x1 >= x2; x1--) route.push_back({ x1, y1 });
				}
				else {
					for (y1; y1 >= i; y1--) route.push_back({ x1, y1 });
					for (x1; x1 >= x2; x1--) route.push_back({ x1, y1 });
				}
				x1++;
				if (flag)
					y1--;
				else
					y1++;
				if (y1 > y2)
					for (y1; y1 >= y2; y1--) route.push_back({ x1, y1 });
				else
					for (y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
				map[tmp_x1][tmp_y1] = 0;
				map[tmp_x2][tmp_y2] = 0;
				return route;
			}
		}
	}
	return route;
}
